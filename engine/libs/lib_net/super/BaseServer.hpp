
#pragma once

#include <atomic>
#include <cstdint>
#include <optional>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>

#include "lib_net/net/Poll.hpp"
#include "lib_net/net/SocketAddr.hpp"
#include "lib_net/net/TcpListener.hpp"
#include "lib_net/net/TcpStream.hpp"
#include "lib_net/net/UdpSocket.hpp"
#include "lib_net/result/Result.hpp"
#include "lib_net/uuid/Uuid.hpp"
#include "lib_net/uuid/UuidGenerator.hpp"

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
    virtual ~BaseServer() = default;

    static auto host(std::uint16_t tcp_port, std::uint16_t udp_port)
        -> result::Result<result::Void, BaseServerError>
    {
        auto tcp_result = net::TcpListener::bind(net::SocketAddr(net::Ipv4Addr(0), tcp_port));
        if (!tcp_result) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::BindError}
            );
        }
        auto udp_result = net::UdpSocket::bind(net::SocketAddr(net::Ipv4Addr(0), udp_port));
        if (!udp_result) {
            return result::Result<result::Void, BaseServerError>::Error(
                BaseServerError {BaseServerError::Kind::BindError}
            );
        }
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
        tcp_connection = tcp_result.value();
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

    auto start() -> result::Result<result::Void, BaseServerError>
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

    void stop()
    {
        _running = false;
        if (_context_thread.joinable()) {
            _context_thread.join();
        }
    }

    // theses functions append the data to the send buffer to a thread safe queue
    void send_tcp(uuid::Uuid id, const std::vector<std::uint8_t> &data);
    void send_udp(const net::SocketAddr &addr, const std::vector<std::uint8_t> &data);

    // called from the context thread
    virtual void on_tcp_connection(uuid::Uuid id) = 0;
    virtual void on_tcp_disconnection(uuid::Uuid id) = 0;

    virtual void on_udp_packet(uuid::Uuid id) = 0;
    virtual void on_tcp_packet(uuid::Uuid id) = 0;

private:
    void handle_events()
    {
        for (const auto &[event, id] : _events) {
            switch (event) {
            case Event::TcpConnection:
                on_tcp_connection(id);
                break;
            case Event::TcpDisconnection:
                on_tcp_disconnection(id);
                break;
            case Event::TcpRead:
                on_tcp_packet(id);
                break;
            case Event::UdpRead:
                on_udp_packet(id);
                break;
            }
        }
        _events.clear();
    }

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
        tcp_connections[id] = stream;
        _events.emplace_back(Event::TcpConnection, id);
    }

    void handle_tcp_read_events(const net::PollEvent &event);
    void handle_tcp_write_events(const net::PollEvent &event);

    void handle_udp_read_event(const net::PollEvent &event);
    void handle_udp_write_event(const net::PollEvent &event);
    // runs in a separate thread (as well as the subsequently called methods) on start()
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
            auto res = poll.add_read(*tcp_connection);
            if (!res) {
                std::cerr << "Poll add read error: " << res.error().message() << std::endl;
                return;
            }
        }
        while (_running) {

            auto events = poll.wait(std::nullopt);
            if (!events) {
                std::cerr << "Poll wait error: " << events.error().message() << std::endl;
                break;
            }

            for (const auto &event : events.value()) {
                switch (event.type) {
                case net::PollEvent::Type::Read:
                    if (udp_socket && event.fd == udp_socket->get_fd()) {
                        handle_udp_read_event(event);
                    } else if (tcp_listener && event.fd == tcp_listener->get_fd()) {
                        handle_new_tcp_connections(poll);
                    } else if (tcp_connection && event.fd == tcp_connection->get_fd()) {
                        // not sure about what to do
                        handle_tcp_read_events(event);
                    } else {
                        handle_tcp_read_events(event);
                    }
                    break;
                case net::PollEvent::Type::Write:
                    if (udp_socket && event.fd == udp_socket->get_fd()) {
                        handle_udp_write_event(event);
                    } else {
                        handle_tcp_write_events(event);
                    }
                    break;
                case net::PollEvent::Type::Error:
                    std::cerr << "Error on fd: " << event.fd << std::endl;
                    break;
                }
            }
            handle_events();
        }
    }

    // should not be accessed/modified once the server is running
    std::optional<net::UdpSocket> udp_socket;
    std::optional<net::TcpListener> tcp_listener;
    std::optional<net::TcpStream> tcp_connection;

    // received connections
    std::unordered_map<uuid::Uuid, net::TcpStream> tcp_connections;

    std::mt19937_64 _generator = std::mt19937_64(std::random_device()());
    uuid::UuidGenerator<decltype(_generator)> tcp_uuid_generator = uuid::UuidGenerator(_generator);

    std::atomic<bool> _running = false;
    std::thread _context_thread;

    // called from external threads

    // contains all events that should be processed by the main thread
    std::vector<std::pair<Event, uuid::Uuid>> _events;
};
}
