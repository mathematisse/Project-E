#include "Server.hpp"
#include <optional>

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
    std::vector<socket_t> fds = {listenFd, udpFd};

    for (auto &[clientId, gateway] : clients) {
        fds.push_back(gateway.tcp_socket.getFD());
        if (gateway.udp_socket.getFD() != INVALID_SOCKET) {
            fds.push_back(gateway.udp_socket.getFD());
        }
    }
    context.select(fds);
    handle_connections();

    std::vector<std::pair<client_id, Packet>> packets_to_process;

    for (auto &[clientId, gateway] : clients) {

        // TCP
        if (context.is_readable(gateway.tcp_socket.getFD())) {
            for (Packet &packet : gateway.tcp_socket.readPackets()) {
                packets_to_process.emplace_back(clientId, packet);
            }
        }
        if (context.is_writable(gateway.tcp_socket.getFD())) {
            gateway.tcp_socket.getBufWriter().appendPackets(gateway.send_tcp_queue);
            gateway.send_tcp_queue.clear();
            ssize_t byte_sent = gateway.tcp_socket.send(gateway.tcp_socket.getBufWriter().getBuffer());
            if (byte_sent == -1) {
                gateway.tcp_socket.close();
            } else {
                gateway.tcp_socket.getBufWriter().consume(byte_sent);
            }
        }

        // // UDP
        // if (context.is_readable(gateway.udp_socket.getFD())) {
        //     for (Packet &packet : gateway.udp_socket.readPackets()) {
        //         packets_to_process.emplace_back(c.first, packet);
        //     }
        // }
        // if (context.is_writable(gateway.udp_socket.getFD())) {
        //     gateway.udp_socket.getBufWriter().appendPackets(gateway.send_udp_queue);
        //     ssize_t byte_sent = gateway.udp_socket.send(gateway.udp_socket.getBufWriter().getBuffer());
        //     gateway.udp_socket.getBufWriter().consume(byte_sent);
        // }
    }

    for (const auto &[clientId, packet] : packets_to_process) {
        on_packet(packet, clientId);
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

void net::Server::handle_connections()
{
    if (context.is_readable(listenFd)) {
        Gateway gateway;
        gateway.tcp_socket = TCPSocket::accept(listenFd);
        gateway.udp_socket.udp_address = {};
        clients.insert({next_client_id, gateway});
        on_connect(next_client_id++);
    }
    for (auto it = clients.begin(); it != clients.end();) {
        if (!it->second.tcp_socket.is_connected()) {
            on_disconnect(it->first);
            it = clients.erase(it);
        } else {
            it++;
        }
    }

    // TODO: UDP connection
}
