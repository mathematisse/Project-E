
#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "lib_net/io/Mutex.hpp"
#include "lib_net/net/Poll.hpp"
#include "lib_net/net/SocketAddr.hpp"
#include "lib_net/net/TcpListener.hpp"
#include "lib_net/net/TcpStream.hpp"
#include "lib_net/net/UdpSocket.hpp"
#include "lib_net/net/_base.hpp"
#include "lib_net/result/Result.hpp"
#include "lib_net/super/TSQueue.hpp"
#include "lib_net/uuid/Uuid.hpp"
#include "lib_net/uuid/UuidGenerator.hpp"
#include "lib_net/io/buffered/BufReader.hpp"
#include "lib_net/io/buffered/BufWriter.hpp"

namespace lnet::utils {

class BaseServerError {
public:
    enum class Kind {
        BindError,
        SocketAlreadyConnected,
        BadIpAddress,
        TcpConnectError,
        ServerAlreadyRunning,
    };

    explicit BaseServerError(Kind type):
        type(type)
    {
    }

    [[nodiscard]]
    auto message() const -> std::string
    {
        switch (type) {
        case Kind::BindError:
            return "Bind error";
        case Kind::SocketAlreadyConnected:
            return "Socket already connected";
        case Kind::BadIpAddress:
            return "Bad IP address";
        case Kind::TcpConnectError:
            return "TCP connect error";
        case Kind::ServerAlreadyRunning:
            return "Server already running";
        }
    }

    Kind type;
};

// every public function should be thread-safe to be called from the main thread
class BaseServer {
public:
    enum class Event {
        TcpRead,
        UdpRead,
        TcpConnection,
        TcpDisconnection,
    };

    BaseServer() = default;
    virtual ~BaseServer()
    {
        // stop the server to be able to access the connections safely
        stop_context();
        // close all existing connections
        for (auto &[id, connection] : tcp_connections) {
            connection.stream.shutdown();
            connection.stream.close();
        }
        if (tcp_listener.has_value()) {
            tcp_listener->close();
        }
        if (udp_socket.has_value()) {
            udp_socket->close();
        }
        if (tcp_connection.has_value()) {
            tcp_connection->stream.shutdown();
            tcp_connection->stream.close();
        }
    }

    auto host_tcp(std::uint16_t port) -> result::Result<result::Void, BaseServerError>
    {
        auto tcp_result = net::TcpListener::bind(net::SocketAddr(net::Ipv4Addr(0), port));
        if (!tcp_result) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::BindError}
            );
        }
        tcp_listener.emplace(tcp_result.value());
        return result::Result<result::Void, BaseServerError>::Success(result::Void {});
    }

    auto host_udp(std::uint16_t port) -> result::Result<result::Void, BaseServerError>
    {
        auto udp_result = net::UdpSocket::bind(net::SocketAddr(net::Ipv4Addr(0), port));
        if (!udp_result) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::BindError}
            );
        }
        udp_socket = udp_result.value();
        return result::Result<result::Void, BaseServerError>::Success(result::Void {});
    }

    auto connect_tcp(const std::string &ip, std::uint16_t port)
        -> result::Result<result::Void, BaseServerError>
    {
        if (_running) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::ServerAlreadyRunning}
            );
        }
        if (tcp_connection.has_value()) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::SocketAlreadyConnected}
            );
        }
        auto address_result = net::Ipv4Addr::parse_ascii(ip);
        if (!address_result) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::BadIpAddress}
            );
        }
        auto address = address_result.value();
        auto tcp_result = net::TcpStream::connect(net::SocketAddr(address, port));
        if (tcp_result.isError()) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::TcpConnectError}
            );
        }
        tcp_connection.emplace(tcp_result.value());
        tcp_connection_id = tcp_uuid_generator.new_uuid();
        return result::Result<result::Void, BaseServerError>::Success(result::Void {});
    }

    auto connect_udp(const std::string &ip, std::uint16_t port)
        -> result::Result<result::Void, BaseServerError>
    {
        if (_running) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::ServerAlreadyRunning}
            );
        }
        if (udp_socket.has_value()) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::SocketAlreadyConnected}
            );
        }
        auto address_result = net::Ipv4Addr::parse_ascii(ip);
        if (!address_result) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::BadIpAddress}
            );
        }
        auto address = address_result.value();
        auto udp_result = net::UdpSocket::bind(net::SocketAddr(address, port));
        if (!udp_result) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::BindError}
            );
        }
        udp_socket = udp_result.value();
        return result::Result<result::Void, BaseServerError>::Success(result::Void {});
    }

    auto start_context() -> result::Result<result::Void, BaseServerError>
    {
        if (_running) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::ServerAlreadyRunning}
            );
        }
        try {
            _running = true;
            _context_thread = std::thread(&BaseServer::run_context, this);
        } catch (const std::system_error &e) {
            _running = false;
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::TcpConnectError}
            );
        }
        return result::Result<result::Void, BaseServerError>::Success(result::Void {});
    }

    void stop_context()
    {
        _running = false;
        if (_context_thread.joinable()) {
            _context_thread.join();
        }
    }

    // theses functions append the data to the send buffer to a thread safe queue
    void send_tcp(uuid::Uuid id, const std::vector<std::uint8_t> &data)
    {
        _send_tcp_queue.push({id, data});
    }

    void send_udp(const net::SocketAddr &addr, const std::vector<std::uint8_t> &data)
    {
        std::vector<std::byte> byte_data(data.size());
        std::transform(data.begin(), data.end(), byte_data.begin(), [](std::uint8_t byte) {
            return static_cast<std::byte>(byte);
        });
        _send_udp_queue.push({addr, byte_data});
    }

    // main thread append to the task queue to be processed by the context thread
    void ask_disconnect(uuid::Uuid id) { _disconnect_queue.push(id); }

    // called from the main thread
    virtual void on_tcp_connection(uuid::Uuid id) = 0;
    virtual void on_tcp_disconnection(uuid::Uuid id) = 0;

    virtual void on_udp_data(const net::SocketAddr &addr, const std::vector<std::byte> &data) = 0;

    virtual void on_tcp_data(uuid::Uuid id, io::Mutex<io::BufReader<net::TcpStream>> &stream) = 0;

    static constexpr auto DEFAULT_TIMEOUT = std::chrono::milliseconds(100);
    // this function is semi-blocking and should be called in a loop
    void handle_events(std::chrono::milliseconds timeout = DEFAULT_TIMEOUT)
    {
        while (!_tcp_events.empty()) {
            auto maybe_event = _tcp_events.wait_pop(timeout);
            if (!maybe_event) {
                break;
            }
            auto [event, id] = maybe_event.value();
            switch (event) {
            case Event::TcpConnection:
                on_tcp_connection(id);
                break;
            case Event::TcpDisconnection:
                on_tcp_disconnection(id);
                ask_disconnect(id);
                break;
            case Event::TcpRead:
                if (tcp_connections.find(id) != tcp_connections.end()) {
                    on_tcp_data(id, tcp_connections.at(id).reader);
                }
                break;
            case Event::UdpRead:
                // Should always come from the same UDP socket
                auto [addr, data] = _recv_udp_queue.wait_pop(timeout).value();
                on_udp_data(addr, data);
                break;
            }
        }
    }

private:
    // Convinience struct to store the tcp connections
    struct TcpConnection {
        net::TcpStream stream;
        lnet::io::Mutex<lnet::io::BufReader<net::TcpStream>> reader;
        lnet::io::BufWriter writer;

        explicit TcpConnection(const net::TcpStream &stream):
            stream(stream),
            reader(lnet::io::BufReader(this->stream)),
            writer(this->stream)
        {
        }

        ~TcpConnection()
        {
            if (auto res = stream.close(); !res) {
                std::cerr << "Tcp close error: " << res.error().message() << std::endl;
            }
        }
    };

    // theses function run in a separate thread on start()
    void handle_new_tcp_connections(net::Poll &poll)
    {
        auto res = tcp_listener->accept();
        if (!res) {
            std::cerr << "Tcp accept error: " << res.error().message() << std::endl;
            return;
        }
        auto [stream, addr] = res.value();
        if (auto res2 = poll.add_read(stream); !res2) {
            std::cerr << "Poll add read error: " << res2.error().message() << std::endl;
            return;
        }
        auto id = tcp_uuid_generator.new_uuid();
        tcp_connections.emplace(id, stream);
        _tcp_events.push({Event::TcpConnection, id});
    }

    void handle_tcp_read_connection()
    {
        tcp_connection->reader.with_lock([&](auto &reader) {
            auto res = reader.fill_buf();
            if (!res) {
                std::cerr << "Tcp read error: " << res.error().message() << std::endl;
                return;
            }
            auto buf = res.value();
            if (buf.empty()) {
                _tcp_events.push({Event::TcpDisconnection, *tcp_connection_id});
                return;
            }
            _tcp_events.push({Event::TcpRead, *tcp_connection_id});
        });
    }

    void handle_tcp_read_events(const net::PollEvent &event)
    {
        auto it = tcp_connections.find(event.fd);
        if (it == tcp_connections.end()) {
            std::cerr << "Tcp connection not found" << std::endl;
            return;
        }
        auto &connection = it->second;
        connection.reader.with_lock([&](auto &reader) {
            auto res = reader.fill_buf();
            if (!res) {
                std::cerr << "Tcp read error: " << res.error().message() << std::endl;
                return;
            }
            auto buf = res.value();
            if (buf.empty()) {
                _tcp_events.push({Event::TcpDisconnection, event.fd});
                return;
            }
            _tcp_events.push({Event::TcpRead, event.fd});
        });
    }

    void handle_tcp_write_events(const net::PollEvent &event)
    {
        auto it = tcp_connections.find(event.fd);
        if (it == tcp_connections.end()) {
            std::cerr << "Tcp connection not found" << std::endl;
            return;
        }
        auto &connection = it->second;
        auto res = connection.writer.flush();
        if (!res) {
            std::cerr << "Tcp write error: " << res.error().message() << std::endl;
            return;
        }
    }

    void handle_tcp_write_connection()
    {
        auto res = tcp_connection->writer.flush();
        if (!res) {
            std::cerr << "Tcp write error: " << res.error().message() << std::endl;
        }
    }

    void handle_udp_read_event()
    {
        auto buffer = std::vector<std::byte>(MAX_BUFFER_SIZE);
        auto span = std::span<std::byte>(buffer);
        auto res = udp_socket->recv_from(span);
        if (!res) {
            std::cerr << "Udp read error: " << res.error().message() << std::endl;
            return;
        }
        auto [size, addr] = res.value();
        auto data = std::vector<std::byte>(buffer.begin(), buffer.begin() + size);
        std::pair<net::SocketAddr, std::vector<std::byte>> event = {addr, data};
        _recv_udp_queue.push(event);
    }

    void handle_udp_write_event()
    {
        auto res = udp_socket->send_to(std::span<std::byte>(), net::SocketAddr());
        if (!res) {
            std::cerr << "Udp write error: " << res.error().message() << std::endl;
        }
    }

    void handle_disconnection()
    {
        while (!_disconnect_queue.empty()) {
            auto id = _disconnect_queue.wait_pop();
            if (id) {
                auto it = tcp_connections.find(*id);
                if (it == tcp_connections.end()) {
                    std::cerr << "Tcp connection not found" << std::endl;
                    continue;
                }
                auto &connection = it->second;
                if (auto res = connection.stream.shutdown(); !res) {
                    std::cerr << "Tcp shutdown error: " << res.error().message() << std::endl;
                }
                if (auto res = connection.stream.close(); !res) {
                    std::cerr << "Tcp close error: " << res.error().message() << std::endl;
                }
                tcp_connections.erase(it);
            }
        }
    }

    void handle_read(const net::PollEvent &event, net::Poll &poll)
    {
        if (udp_socket && event.fd == udp_socket->get_fd()) {
            handle_udp_read_event();
        } else if (tcp_listener && event.fd == tcp_listener->get_fd()) {
            handle_new_tcp_connections(poll);
        } else if (tcp_connection && event.fd == (*tcp_connection).stream.get_fd()) {
            handle_tcp_read_connection();
        } else {
            handle_tcp_read_events(event);
        }
    }

    void handle_write(const net::PollEvent &event)
    {
        if (udp_socket && event.fd == udp_socket->get_fd()) {
            handle_udp_write_event();
        } else if (tcp_connection && event.fd == (*tcp_connection).stream.get_fd()) {
            handle_tcp_write_connection();
        } else {
            handle_tcp_write_events(event);
        }
    }

    void context_loop(net::Poll &poll)
    {
        while (_running) {
            auto events = poll.wait(std::nullopt);
            if (!events) {
                std::cerr << "Poll wait error: " << events.error().message() << std::endl;
                break;
            }

            for (const auto &event : events.value()) {
                switch (event.type) {
                case net::PollEvent::Type::Read:
                    handle_read(event, poll);
                    break;
                case net::PollEvent::Type::Write:
                    handle_write(event);
                    break;
                case net::PollEvent::Type::Error:
                    std::cerr << "Error on fd: " << event.fd << std::endl;
                    break;
                }
            }
            handle_disconnection();
        }
    }

    void run_context()
    {
        net::Poll poll;

        if (udp_socket) {
            auto res = poll.add_read(*udp_socket);
            if (!res) {
                std::cerr << "Poll add read error: " << res.error().message() << std::endl;
                return;
            }
        }
        if (tcp_listener) {
            auto res = poll.add_read(*tcp_listener);
            if (!res) {
                std::cerr << "Poll add read error: " << res.error().message() << std::endl;
                return;
            }
        }
        if (tcp_connection) {
            auto res = poll.add_read((*tcp_connection).stream);
            if (!res) {
                std::cerr << "Poll add read error: " << res.error().message() << std::endl;
                return;
            }
        }
        context_loop(poll);
    }

    // should not be accessed/modified once the server is running
    std::optional<net::UdpSocket> udp_socket;
    std::optional<net::TcpListener> tcp_listener;
    std::optional<TcpConnection> tcp_connection;
    std::optional<uuid::Uuid> tcp_connection_id;

    // received connections
    std::unordered_map<uuid::Uuid, TcpConnection> tcp_connections;

    std::mt19937_64 _generator = std::mt19937_64(std::random_device()());
    // used to generate unique ids for each connection
    uuid::UuidGenerator<decltype(_generator)> tcp_uuid_generator = uuid::UuidGenerator(_generator);

    // ----------------- used from external threads ------------------------
    std::atomic<bool> _running = false;
    std::thread _context_thread;

    // contains all events that should be processed by the main thread
    TsQueue<std::pair<Event, uuid::Uuid>> _tcp_events;
    // doesn't need to be thread safe since it's only accessed coupled with _tcp_events
    TsQueue<std::pair<net::SocketAddr, std::vector<std::byte>>> _recv_udp_queue;

    // contains all data that should be sent to the context thread
    TsQueue<std::pair<uuid::Uuid, std::vector<std::uint8_t>>> _send_tcp_queue;
    TsQueue<std::pair<net::SocketAddr, std::vector<std::byte>>> _send_udp_queue;

    // contains all disconnection requests
    TsQueue<uuid::Uuid> _disconnect_queue;
};
}
