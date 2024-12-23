#include <algorithm>
#include <atomic>
#include <cstdint>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "lib_log/log.hpp"
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
#include "lib_net/io/buffered/BufWriter.hpp"
#include "lib_net/super/BaseServer.hpp"

namespace lnet::utils {

BaseServer::~BaseServer()
{
    // stop the server to be able to access the connections safely
    stop_context();
    // close all existing connections
    for (auto &[id, connection] : tcp_connections) {
        if (auto res = connection.stream.close(); !res) {
            LOG_DEBUG("Tcp close error: " + res.error().message());
        }
    }
    if (tcp_listener.has_value()) {
        if (auto res = tcp_listener->close(); !res) {
            LOG_DEBUG("Tcp listener close error: " + res.error().message());
        }
    }
    if (udp_socket.has_value()) {
        if (auto res = udp_socket->close(); !res) {
            LOG_DEBUG("Udp socket close error: " + res.error().message());
        }
    }
    if (tcp_connection.has_value()) {
        if (auto res = tcp_connection->stream.close(); !res) {
            LOG_DEBUG("Tcp connection close error: " + res.error().message());
        }
    }
}

auto BaseServer::host_tcp(std::uint16_t port) -> result::Result<result::Void, BaseServerError>
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

auto BaseServer::host_udp(std::uint16_t port) -> result::Result<result::Void, BaseServerError>
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

auto BaseServer::connect_tcp(const std::string &ip, std::uint16_t port)
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
    LOG_INFO("Connecting to " + address.to_string() + ":" + std::to_string(port));
    auto tcp_result = net::TcpStream::connect(net::SocketAddr(address, port));
    if (tcp_result.isError()) {
        LOG_INFO("Tcp connect error: " + tcp_result.error().message());
        return result::Result<result::Void, BaseServerError>::Error(
            BaseServerError {BaseServerError::Kind::TcpConnectError}
        );
    }
    tcp_connection.emplace(tcp_result.value());
    tcp_connection_id = tcp_uuid_generator.new_uuid();
    _tcp_events.push({TCPEvent::TcpConnection, *tcp_connection_id});
    return result::Result<result::Void, BaseServerError>::Success(result::Void {});
}

auto BaseServer::connect_udp(const std::string &ip, std::uint16_t port)
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
    auto udp_result = net::UdpSocket::any(AF_INET);
    if (!udp_result) {
        return result::Result<result::Void, BaseServerError>::Error(
            BaseServerError {BaseServerError::Kind::BindError}
        );
    }
    udp_socket = udp_result.value();
    // get the address of the port of the socket that was just created
    auto socket_addr = udp_socket->local_addr();
    if (!socket_addr) {
        return result::Result<result::Void, BaseServerError>::Error(
            BaseServerError {BaseServerError::Kind::BindError}
        );
    }
    auto address_s = socket_addr.value();
    LOG_INFO("UDP socket bound to " + address_s.to_string());

    udp_connection_addr = net::SocketAddr(address, port);
    // _recv_udp_queue.push({*udp_connection_addr, std::vector<std::uint8_t> {}});
    _udp_confirmed = true;
    return result::Result<result::Void, BaseServerError>::Success(result::Void {});
}

auto BaseServer::start_context() -> result::Result<result::Void, BaseServerError>
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

void BaseServer::stop_context()
{
    _running = false;
    if (_context_thread.joinable()) {
        _context_thread.join();
    }
}

// theses functions append the data to the send buffer to a thread safe queue
void BaseServer::send_tcp(uuid::Uuid id, const std::vector<std::uint8_t> &data)
{
    _send_tcp_queue.push({id, data});
}

void BaseServer::send_udp(const net::SocketAddr &addr, const std::vector<std::uint8_t> &data)
{
    _send_udp_queue.push({addr, data});
}

void BaseServer::send_udp(const std::vector<std::uint8_t> &data)
{
    if (udp_connection_addr.has_value()) {
        send_udp(udp_connection_addr.value(), data);
    } else {
        // ERROR
        LOG_DEBUG("No UDP connection address");
    }
}

// main thread append to the task queue to be processed by the context thread
void BaseServer::ask_disconnect(uuid::Uuid id) { _disconnect_queue.push(id); }

void BaseServer::handle_events(std::chrono::milliseconds timeout)
{
    while (!_tcp_events.empty()) {
        auto maybe_event = _tcp_events.wait_pop(timeout);
        if (!maybe_event) {
            break;
        }
        auto [event, id] = maybe_event.value();
        switch (event) {
        case TCPEvent::TcpConnection:
            on_tcp_connection(id);
            break;
        case TCPEvent::TcpDisconnection:
            on_tcp_disconnection(id);
            break;
        case TCPEvent::TcpRead:
            if (tcp_connection.has_value() && *tcp_connection_id == id) {
                on_tcp_data(id, tcp_connection->reader);
            } else if (tcp_connections.find(id) != tcp_connections.end()) {
                std::lock_guard guard(using_tcp_connection);
                on_tcp_data(id, tcp_connections.at(id).reader);
            } else {
                LOG_DEBUG("TCP connection for data even not found");
            }
            break;
        } // Should always come from the same UDP socket
    }

    while (!_recv_udp_queue.empty()) {
        auto ret = _recv_udp_queue.wait_pop(timeout);
        if (ret) {
            auto [addr, data] = ret.value();
            on_udp_data(addr, data);
        }
    }
}

// theses function run in a separate thread on start()
void BaseServer::handle_new_tcp_connections(net::Poll &poll)
{
    auto res = tcp_listener->accept();
    if (!res) {
        LOG_DEBUG("Tcp accept error: " + res.error().message());
        return;
    }
    auto [stream, addr] = res.value();
    LOG_INFO("New TCP connection accepted fd: " + std::to_string(stream.get_fd()));
    if (auto res2 = poll.add(stream); !res2) {
        LOG_DEBUG("Poll add read error: " + res2.error().message());
        if (auto res_ = stream.close(); !res_) {
            // LOG_DEBUG("Tcp close error: " + res_.error().message());
        }
        return;
    }
    // Maybe remove this?
    if (auto res2 = stream.set_nonblocking(true); !res2) {
        LOG_DEBUG("Tcp set nonblocking error: " + res2.error().message());
        if (auto res_ = stream.close(); !res_) {
            // LOG_DEBUG("Tcp close error: " + res_.error().message());
        }
        return;
    }
    auto id = tcp_uuid_generator.new_uuid();
    tcp_connections.emplace(id, stream);
    _tcp_events.push({TCPEvent::TcpConnection, id});
}

void BaseServer::handle_tcp_read_connection(net::Poll & /*poll*/)
{
    tcp_connection->reader.with_lock([&](auto &reader) {
        auto res = reader.fill_buf();
        if (!res) {
            LOG_DEBUG("Tcp read error: " + res.error().message());
            return;
        }
        auto buf = res.value();
        if (buf.empty()) {
            return;
        }
        _tcp_events.push({TCPEvent::TcpRead, *tcp_connection_id});
    });
}

auto BaseServer::get_tcp_connection(int fd
) -> std::optional<std::unordered_map<uuid::Uuid, TcpConnection>::iterator>
{
    auto it = std::find_if(tcp_connections.begin(), tcp_connections.end(), [fd](const auto &pair) {
        return pair.second.stream.get_fd() == fd;
    });
    if (it == tcp_connections.end()) {
        return std::nullopt;
    }
    return it;
}

void BaseServer::handle_tcp_read_events(const net::PollEvent &event, net::Poll &poll)
{
    auto maybe_connection = get_tcp_connection(event.fd);
    if (!maybe_connection) {
        LOG_DEBUG("Tcp connection not found");
        return;
    }
    auto &[id, connection] = *maybe_connection.value();
    connection.reader.with_lock([&](auto &reader) {
        auto res = reader.fill_buf();
        if (!res) {
            LOG_DEBUG("Tcp read error: " + res.error().message());
            return;
        }
        auto buf = res.value();
        if (buf.empty()) {
            disconnect_tcp_connection(id, poll);
            return;
        }
        _tcp_events.push({TCPEvent::TcpRead, id});
    });
}

void BaseServer::handle_tcp_write_events(const net::PollEvent &event, net::Poll &poll)
{
    auto maybe_connection = get_tcp_connection(event.fd);
    if (!maybe_connection) {
        LOG_DEBUG("Tcp connection not found");
        return;
    }
    auto &[id, connection] = *maybe_connection.value();
    auto res = connection.writer.flush();
    if (!res) {
        LOG_DEBUG("Tcp write error: " + res.error().message());
        disconnect_tcp_connection(id, poll);
        return;
    }
}

void BaseServer::handle_tcp_write_connection(net::Poll &poll)
{
    auto res = tcp_connection->writer.flush();
    if (!res) {
        LOG_DEBUG("Tcp write error: " + res.error().message());
        disconnect_tcp_connection_single(poll);
    }
}

void BaseServer::handle_udp_read_event()
{
    auto buffer = std::vector<std::uint8_t>(MAX_BUFFER_SIZE);
    auto span = std::span<std::uint8_t>(buffer);
    auto res = udp_socket->recv_from(span);
    if (!res) {
        LOG_DEBUG("Udp read error: " + res.error().message());
        return;
    }
    auto [size, addr] = res.value();
    if (size == 0) {
        return;
    }
    if (!is_valid_udp_message(buffer)) {
        return;
    }
    std::pair<net::SocketAddr, std::vector<std::uint8_t>> event = {
        addr, std::vector<std::uint8_t>(buffer.begin(), buffer.begin() + (long) size)
    };
    _recv_udp_queue.push(event);
}

void BaseServer::handle_udp_write_event()
{
    if (_send_udp_queue.empty()) {
        return;
    }
    auto maybe_data = _send_udp_queue.wait_pop(std::chrono::milliseconds(1));
    if (maybe_data) {
        auto [addr, data] = maybe_data.value();
        if (udp_connection_addr.has_value()) {
            addr = udp_connection_addr.value();
        }
        auto res = udp_socket->send_to(data, addr);
        if (!res) {
            LOG_DEBUG("Udp send error: " + res.error().message());
        }
    }
}

void BaseServer::disconnect_tcp_connection_single(net::Poll &poll)
{
    if (tcp_connection) {
        auto &connection = *tcp_connection;
        LOG_INFO("About to Disconnect client (" + (*tcp_connection_id).to_str() + ")");
        if (auto res = poll.remove(connection.stream); !res) {
            // LOG_DEBUG("Poll remove read error: " + res.error().message());
        }
        if (auto res = connection.stream.close(); !res) {
            // LOG_DEBUG("Tcp close error: " + res.error().message());
        }
        _tcp_events.push({TCPEvent::TcpDisconnection, *tcp_connection_id});
        tcp_connection.reset();
        tcp_connection_id.reset();
    }
}

// direclty disconnect the client
void BaseServer::disconnect_tcp_connection(uuid::Uuid id, net::Poll &poll)
{
    auto it = tcp_connections.find(id);
    if (it == tcp_connections.end()) {
        LOG_DEBUG("Tcp connection not found");
        return;
    }
    auto &connection = it->second;
    LOG_INFO("About to Disconnect client (" + it->first.to_str() + ")");
    if (auto res = poll.remove(connection.stream); !res) {
        // LOG_DEBUG("Poll remove read error: " + res.error().message());
    }
    if (auto res = connection.stream.close(); !res) {
        // LOG_DEBUG("Tcp close error: " + res.error().message());
    }
    {
        std::lock_guard guard(using_tcp_connection);
        tcp_connections.erase(it);
    }
    _tcp_events.push({TCPEvent::TcpDisconnection, id});
}

void BaseServer::handle_disconnection(net::Poll &poll)
{
    while (!_disconnect_queue.empty()) {
        auto maybe_uuid = _disconnect_queue.wait_pop();
        if (maybe_uuid) {
            disconnect_tcp_connection(maybe_uuid.value(), poll);
        }
    }
}

void BaseServer::handle_read(const net::PollEvent &event, net::Poll &poll)
{
    if (udp_socket && event.fd == udp_socket->get_fd()) {
        handle_udp_read_event();
    } else if (tcp_listener && event.fd == tcp_listener->get_fd()) {
        handle_new_tcp_connections(poll);
    } else if (tcp_connection && event.fd == (*tcp_connection).stream.get_fd()) {
        handle_tcp_read_connection(poll);
    } else {
        handle_tcp_read_events(event, poll);
    }
}

void BaseServer::handle_write(const net::PollEvent &event, net::Poll &poll)
{
    if (udp_socket && event.fd == udp_socket->get_fd()) {
        handle_udp_write_event();
    } else if (tcp_connection && event.fd == (*tcp_connection).stream.get_fd()) {
        handle_tcp_write_connection(poll);
    } else {
        handle_tcp_write_events(event, poll);
    }
}

void BaseServer::prepare_write(net::Poll &poll)
{
    handle_tcp_connection_write(poll);
    handle_tcp_connections_write(poll);
    handle_udp_socket_write(poll);
}

void BaseServer::handle_tcp_connection_write(net::Poll &poll)
{
    if (tcp_connection) {
        auto &connection = *tcp_connection;
        if (!connection.writer.is_empty()) {
            auto res = poll.add_write(connection.stream);
            if (!res) {
                LOG_DEBUG("Poll add write error: " + res.error().message());
            }
        } else {
            if (auto res = poll.remove_write(connection.stream); !res) {
                LOG_DEBUG("Poll remove TCP Connection write error: " + res.error().message());
            }
        }
    }
}

void BaseServer::handle_tcp_connections_write(net::Poll &poll)
{
    for (const auto &[id, connection] : tcp_connections) {
        if (!connection.writer.is_empty()) {
            auto res = poll.add_write(connection.stream);
            if (!res) {
                LOG_DEBUG("Poll add write error: " + res.error().message());
            }
        } else {
            if (auto res = poll.remove_write(connection.stream); !res) {
                LOG_DEBUG(
                    "Poll remove TCP ()" + id.to_str() + ") write error: " + res.error().message()
                );
            }
        }
    }
}

void BaseServer::handle_udp_socket_write(net::Poll &poll)
{
    if (udp_socket) {
        if (!_send_udp_queue.empty()) {
            auto res = poll.add_write(*udp_socket);
            if (!res) {
                LOG_DEBUG("Poll add write error: " + res.error().message());
            }
        } else {
            if (auto res = poll.remove_write(*udp_socket); !res) {
                LOG_DEBUG("Poll remove UDP write error: " + res.error().message());
            }
        }
    }
}

void BaseServer::handle_error(const net::PollEvent &event, net::Poll &poll)
{
    if (udp_socket && event.fd == udp_socket->get_fd()) {
        LOG_DEBUG("Error on UDP socket");
    } else if (tcp_listener && event.fd == tcp_listener->get_fd()) {
        LOG_DEBUG("Error on TCP listener");
    } else if (tcp_connection && event.fd == (*tcp_connection).stream.get_fd()) {
        LOG_DEBUG("Error on TCP connection");
        disconnect_tcp_connection_single(poll);
    } else {
        auto maybe_connection = get_tcp_connection(event.fd);
        if (!maybe_connection) {
            LOG_DEBUG("Error on TCP connection");
            return;
        }
        auto &[id, connection] = *maybe_connection.value();
        LOG_DEBUG("Error on TCP connection (" + id.to_str() + ")");
        disconnect_tcp_connection(id, poll);
    }
}

void BaseServer::context_loop(net::Poll &poll)
{
    while (_running) {
        prepare_write(poll);

        auto events = poll.wait(1 /*ms*/);
        if (!events) {
            LOG_DEBUG("Poll wait error: " + events.error().message());
            break;
        }

        for (const auto &event : events.value()) {
            switch (event.type) {
            case net::PollEvent::Type::Read:
                handle_read(event, poll);
                break;
            case net::PollEvent::Type::Write:
                handle_write(event, poll);
                break;
            case net::PollEvent::Type::Error:
                handle_error(event, poll);
                break;
            }
        }
        handle_disconnection(poll);
        // append the data to all the tcp connections
        while (!_send_tcp_queue.empty()) {
            auto maybe_data = _send_tcp_queue.wait_pop();
            if (maybe_data) {
                auto [id, data] = maybe_data.value();
                if (auto it = tcp_connections.find(id); it != tcp_connections.end()) {
                    auto &connection = it->second;
                    auto ret = connection.writer.write(data);
                    if (!ret) {
                        LOG_DEBUG("Tcp write error: " + ret.error().message());
                    }
                }
            }
        }
    }
}

void BaseServer::run_context()
{
    net::Poll poll;

    if (udp_socket) {
        auto res = poll.add(*udp_socket);
        if (!res) {
            LOG_DEBUG("Poll add read error: " + res.error().message());
            return;
        }
    }
    if (tcp_listener) {
        auto res = poll.add(*tcp_listener);
        if (!res) {
            LOG_DEBUG("Poll add read error: " + res.error().message());
            return;
        }
    }
    if (tcp_connection) {
        auto res = poll.add(tcp_connection->stream);
        if (!res) {
            LOG_DEBUG("Poll add read error: " + res.error().message());
            return;
        }
    }
    context_loop(poll);
}
}
