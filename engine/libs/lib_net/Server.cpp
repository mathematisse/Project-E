#include "Server.hpp"
#include "lib_net/Context.hpp"
#include "lib_net/Gateway.hpp"
#include "lib_net/Packet.hpp"
#include <iostream>
#include <optional>
#include <vector>

bool net::Server::host(std::uint16_t port) { return host_tcp(port) && host_udp(port); }

void net::Server::send_tcp(client_id id, Packet::MsgType type, const std::vector<std::uint8_t> &data)
{
    if (auto opt_gateway = get_gateway(id); opt_gateway.has_value()) {
        Gateway &gateway = opt_gateway.value();
        gateway.send_tcp_queue.push_back(Packet::deserialize(type, data));
    }
}

void net::Server::send_udp(client_id id, Packet::MsgType type, const std::vector<std::uint8_t> &data)
{
    if (auto opt_gateway = get_gateway(id); opt_gateway.has_value()) {
        Gateway &gateway = opt_gateway.value();
        gateway.send_udp_queue.push_back(Packet::deserialize(type, data));
    }
}

void net::Server::send_tcp(Packet::MsgType type, const std::vector<std::uint8_t> &data)
{
    for (auto &[clientId, gateway] : clients) {
        gateway.send_tcp_queue.push_back(Packet::deserialize(type, data));
    }
}

void net::Server::send_udp(Packet::MsgType type, const std::vector<std::uint8_t> &data)
{
    for (auto &[clientId, gateway] : clients) {
        gateway.send_udp_queue.push_back(Packet::deserialize(type, data));
    }
}

std::optional<std::reference_wrapper<net::Gateway>> net::Server::get_gateway(client_id id)
{
    auto it = clients.find(id);

    if (it == clients.end()) {
        return std::nullopt;
    }
    return std::ref(it->second);
}

void net::Server::update()
{
    // always listen for new connections and udp packets
    std::vector<socket_t> read_fds = {listenFd, udpFd};
    std::vector<socket_t> write_fds = {};
    bool should_add_udp = false;

    for (auto &[clientId, gateway] : clients) {
        const auto &fd = gateway.tcp_socket.getFD();
        if (fd != INVALID_SOCKET) {
            read_fds.push_back(fd);
            // if there is something to send, add the socket to the write_fds
            if (!gateway.send_tcp_queue.empty()) {
                write_fds.push_back(fd);
            }
        }
        // append the udp socket to the write_fds if there is something to send in any of the clients
        if (!gateway.send_udp_queue.empty()) {
            should_add_udp = true;
        }
    }
    if (should_add_udp) {
        write_fds.push_back(udpFd);
    }
    context.select(read_fds, write_fds);
    handle_new_tcp_connections();

    for (auto &[clientId, gateway] : clients) {

        if (context.is_readable(gateway.tcp_socket.getFD())) {
            context.readyCount--;
            if (gateway.tcp_socket.recvToBuffer() == 0) {
                gateway.tcp_socket.close();
            }
        }
        if (context.is_writable(gateway.tcp_socket.getFD())) {
            context.readyCount--;
            gateway.tcp_socket.getBufWriter().appendPackets(gateway.send_tcp_queue);
            gateway.send_tcp_queue.clear();
            ssize_t byte_sent = gateway.tcp_socket.send(gateway.tcp_socket.getBufWriter().getBuffer());
            if (byte_sent < 0) {
                gateway.tcp_socket.close();
            } else {
                gateway.tcp_socket.getBufWriter().consume(byte_sent);
            }
        }
        if (context.readyCount <= 0) {
            break;
        }
    }

    if (context.readyCount <= 0) {
        return;
    }

    // should only treat the first packet if the client has already an existing TCP connection
    if (context.is_readable(udpFd)) {
        context.readyCount--;
        sockaddr_in addr {};
        std::vector<std::uint8_t> buffer(MAX_BUFFER_SIZE);
        socklen_t addr_len = sizeof(addr);
        ssize_t bytes_received = recvfrom(
            udpFd, reinterpret_cast<char *>(buffer.data()), buffer.size(), 0,
            reinterpret_cast<struct sockaddr *>(&addr), &addr_len
        );
        if (bytes_received > 0) {
            // check if the client already has a UDP connection stored
            if (auto it = std::find_if(
                    clients.begin(), clients.end(),
                    [&addr](const auto &client) {
                        return client.second.udp_info.is_identic(addr);
                    }
                );
                it != clients.end()) {
                auto &gateway = it->second;
                buffer.resize(bytes_received);
                gateway.udp_info.buf_reader.append(buffer);
            } else {
                // new udp connection, deserialize packet, check if the packet has the correct number & find
                // the client with the same generated number
                if (auto packet = Packet::deserialize(buffer); packet.has_value()) {
                    if (packet->header.type == Packet::SystemTypes::ASKUDP_RESPONSE &&
                        packet->data.size() == sizeof(std::uint64_t)) {

                        // data section of the packet is the generated number
                        std::uint64_t number = *reinterpret_cast<std::uint64_t *>(packet->data.data());
                        if (auto it = std::find_if(
                                clients.begin(), clients.end(),
                                [number, addr](const auto &client) {
                                    return Gateway::transformNumberFunction(client.second.generated_number) ==
                                        number &&
                                        client.second.tcp_socket.is_same_address(addr);
                                }
                            );
                            it != clients.end()) {
                            it->second.udp_info.udp_address = addr;
                            on_udp_connect(it->first);
                        }
                    }
                } else {
                    // Do nothing as the client does not respect the protocol (ignore the packet)
                }
            }
        }
    }

    if (context.is_writable(udpFd)) {
        context.readyCount--;
        for (auto &[clientId, gateway] : clients) {
            // if the udp_address is not set, ignore the client
            if (gateway.udp_info.udp_address.sin_port == 0U) {
                continue;
            }

            gateway.udp_info.buf_writer.appendPackets(gateway.send_udp_queue);
            gateway.send_udp_queue.clear();
            ssize_t byte_sent = sendto(
                udpFd, reinterpret_cast<const char *>(gateway.udp_info.buf_writer.getBuffer().data()),
                gateway.udp_info.buf_writer.getBuffer().size(), 0,
                reinterpret_cast<struct sockaddr *>(&gateway.udp_info.udp_address),
                sizeof(gateway.udp_info.udp_address)
            );
            if (byte_sent < 0) {
                gateway.tcp_socket.close();
            } else {
                gateway.udp_info.buf_writer.consume(byte_sent);
            }

            // break early if no more ready events
            if (context.readyCount <= 0) {
                return;
            }
        }
    }

    // can be done in another thread
    for (auto it = clients.begin(); it != clients.end();) {
        if (!it->second.tcp_socket.is_connected()) {
            on_tcp_disconnect(it->first);
            it = clients.erase(it);
        } else {
            ++it;
        }
    }

    for (auto &[clientId, gateway] : clients) {
        for (Packet &packet : gateway.tcp_socket.readPackets()) {
            on_packet(packet, clientId);
        }
        for (Packet &packet : gateway.udp_info.readPackets()) {
            on_packet(packet, clientId);
        }
    }
}

bool net::Server::host_tcp(std::uint16_t port)
{
#ifdef _WIN32 // Windows
    struct addrinfo hints { };
    struct addrinfo *result = nullptr;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &result) != 0) {
        return false;
    }
    listenFd = socket(result.ai_family, result.ai_socktype, result.ai_protocol);
    if (listenFd == INVALID_SOCKET) {
        freeaddrinfo(result);
        return false;
    }
    if (bind(listenFd, result->ai_addr, static_cast<int>(result->ai_addrlen)) == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(listenFd);
        return false;
    }
    freeaddrinfo(result);
    if (listen(listenFd, MAX_LISTEN_QUEUE_SIZE) == SOCKET_ERROR) {
        closesocket(listenFd);
        return false;
    }

#else // Linux
    sockaddr_in addr {};

    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd == INVALID_SOCKET) {
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(listenFd, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
        return false;
    }
    if (listen(listenFd, MAX_LISTEN_QUEUE_SIZE) != 0) {
        return false;
    }

#endif

    return true;
}

bool net::Server::host_udp(std::uint16_t port)
{
#ifdef _WIN32 // Windows
    struct addrinfo hints { };
    struct addrinfo *result = nullptr;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &result) != 0) {
        WSACleanup();
        return false;
    }
    udpFd = socket(result.ai_family, result.ai_socktype, result.ai_protocol);
    if (udpFd == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }
    if (bind(udpFd, result->ai_addr, static_cast<int>(result->ai_addrlen)) == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(udpFd);
        WSACleanup();
        return false;
    }
    freeaddrinfo(result);

#else // Linux
    sockaddr_in addr {};

    udpFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpFd == INVALID_SOCKET) {
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(udpFd, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
        return false;
    }

#endif

    return true;
}

void net::Server::handle_new_tcp_connections()
{
    if (context.is_readable(listenFd)) {
        context.readyCount--;
        Gateway gateway;
        if (auto opt_socket = TCPSocket::accept(listenFd); opt_socket.has_value()) {
            gateway.tcp_socket = opt_socket.value();
            if (gateway.tcp_socket.getFD() == INVALID_SOCKET) {
                std::cerr << "Error accepting connection" << std::endl;
                return;
            }
            gateway.udp_info.udp_address = {};
            clients.insert({next_client_id, gateway});
            on_tcp_connect(next_client_id++);
        }
    }
}
