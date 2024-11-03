
#pragma once

#include <atomic>
#include <cstdint>
#include <iostream>
#include <memory>
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
        default:
            return "Unknown error";
        }
    }

    Kind type;
};

// every public function should be thread-safe to be called from the main thread
class BaseServer {
public:
    enum class TCPEvent {
        TcpRead,
        TcpConnection,
        TcpDisconnection,
    };

    BaseServer() = default;
    virtual ~BaseServer();

    // called from the main thread
    virtual void on_tcp_connection(uuid::Uuid id) = 0;
    virtual void on_tcp_disconnection(uuid::Uuid id) = 0;
    virtual bool is_valid_udp_message(const std::vector<std::uint8_t> &data) = 0;
    virtual void
    on_udp_data(const net::SocketAddr &addr, const std::vector<std::uint8_t> &data) = 0;
    virtual void on_tcp_data(uuid::Uuid id, io::Mutex<io::BufReader<net::TcpStream>> &stream) = 0;

    auto host_tcp(std::uint16_t port) -> result::Result<result::Void, BaseServerError>;
    auto host_udp(std::uint16_t port) -> result::Result<result::Void, BaseServerError>;
    auto connect_tcp(const std::string &ip, std::uint16_t port)
        -> result::Result<result::Void, BaseServerError>;

    auto connect_udp(const std::string &ip, std::uint16_t port)
        -> result::Result<result::Void, BaseServerError>;
    auto start_context() -> result::Result<result::Void, BaseServerError>;
    void stop_context();
    // theses functions append the data to the send buffer to a thread safe queue
    void send_tcp(uuid::Uuid id, const std::vector<std::uint8_t> &data);
    void send_udp(const net::SocketAddr &addr, const std::vector<std::uint8_t> &data);
    void send_udp(const std::vector<std::uint8_t> &data);
    // main thread append to the task queue to be processed by the context thread
    void ask_disconnect(uuid::Uuid id);

    static constexpr auto DEFAULT_TIMEOUT = std::chrono::milliseconds(5);
    // this function is semi-blocking and should be called in a loop
    void handle_events(std::chrono::milliseconds timeout = DEFAULT_TIMEOUT);

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
    void handle_new_tcp_connections(net::Poll &poll);
    void handle_tcp_read_connection(net::Poll &poll);
    auto get_tcp_connection(int fd
    ) -> std::optional<std::unordered_map<uuid::Uuid, TcpConnection>::iterator>;
    void handle_tcp_read_events(const net::PollEvent &event, net::Poll &poll);
    void handle_tcp_write_events(const net::PollEvent &event, net::Poll &poll);

    void handle_tcp_write_connection(net::Poll &poll);
    void handle_udp_read_event();
    void handle_udp_write_event();
    void disconnect_tcp_connection_single(net::Poll &poll);
    // direclty disconnect the client
    void disconnect_tcp_connection(uuid::Uuid id, net::Poll &poll);

    void handle_disconnection(net::Poll &poll);

    void handle_read(const net::PollEvent &event, net::Poll &poll);

    void handle_write(const net::PollEvent &event, net::Poll &poll);
    void prepare_write(net::Poll &poll);
    void handle_tcp_connection_write(net::Poll &poll);
    void handle_tcp_connections_write(net::Poll &poll);
    void handle_udp_socket_write(net::Poll &poll);
    void handle_error(const net::PollEvent &event, net::Poll &poll);

    void context_loop(net::Poll &poll);

    void run_context();
    // should not be accessed/modified once the server is running

    // Hosted connections
    std::optional<net::UdpSocket> udp_socket;
    std::optional<net::TcpListener> tcp_listener;

    // Connected connections
    std::optional<TcpConnection> tcp_connection;
    std::optional<uuid::Uuid> tcp_connection_id;
    std::optional<net::SocketAddr> udp_connection_addr;

protected:
    std::atomic_bool _udp_confirmed = false;

private:
    // received connections
    std::unordered_map<uuid::Uuid, TcpConnection> tcp_connections;

    std::shared_ptr<std::mt19937_64> _generator =
        std::make_shared<std::mt19937_64>(std::random_device()());
    // used to generate unique ids for each connection
    uuid::UuidGenerator<> tcp_uuid_generator = uuid::UuidGenerator(_generator);

    // ----------------- used from external threads ------------------------
    std::atomic<bool> _running = false;
    std::thread _context_thread;

    // contains all events that should be processed by the main thread
    TsQueue<std::pair<TCPEvent, uuid::Uuid>> _tcp_events;
    // doesn't need to be thread safe since it's only accessed coupled with _tcp_events
    TsQueue<std::pair<net::SocketAddr, std::vector<std::uint8_t>>> _recv_udp_queue;

    // contains all data that should be sent to the context thread
    TsQueue<std::pair<uuid::Uuid, std::vector<std::uint8_t>>> _send_tcp_queue;
    TsQueue<std::pair<net::SocketAddr, std::vector<std::uint8_t>>> _send_udp_queue;

    // contains all disconnection requests
    TsQueue<uuid::Uuid> _disconnect_queue;

    std::mutex using_tcp_connection;
};
}
