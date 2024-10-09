#include "Server.hpp"

bool net::Server::host(std::uint16_t port) { return host_tcp(port) && host_udp(port); }

void net::Server::send_tcp(client_id id, Packet::MsgType type, const std::vector<std::uint8_t> &data)
{
    std::optional<Gateway *> opt_gateway = get_gateway(id);

    if (!opt_gateway.has_value()) {
        return;
    }
    Gateway &gateway = *opt_gateway.value();
    gateway.send_tcp_queue.push_back(Packet::deserialize(type, data));
}

void net::Server::send_udp(client_id id, Packet::MsgType type, const std::vector<std::uint8_t> &data)
{
    std::optional<Gateway *> opt_gateway = get_gateway(id);

    if (!opt_gateway.has_value()) {
        return;
    }
    Gateway &gateway = *opt_gateway.value();
    gateway.send_udp_queue.push_back(Packet::deserialize(type, data));
}

std::optional<net::Gateway *> net::Server::get_gateway(client_id id)
{
    auto it = clients.find(id);

    if (it == clients.end()) {
        return std::nullopt;
    }
    return &it->second;
}

void net::Server::update()
{
    std::vector<socket_t> fds = {listenFd, udpFd};

    for (auto &c : clients) {
        fds.push_back(c.second.tcp_socket.getFD());
        fds.push_back(c.second.udp_socket.getFD());
    }
    context.select(fds);
    handle_connections();
    for (auto &c : clients) {
        Gateway &gateway = c.second;

        // TCP
        if (context.is_readable(gateway.tcp_socket.getFD())) {
            for (Packet &packet : gateway.tcp_socket.readPackets()) {
                on_packet(packet, c.first);
            }
        }
        if (context.is_writable(gateway.tcp_socket.getFD())) {
            gateway.tcp_socket.getBufWriter().appendPackets(gateway.send_tcp_queue);
            ssize_t byte_sent = gateway.tcp_socket.send(gateway.tcp_socket.getBufWriter().getBuffer());
            gateway.tcp_socket.getBufWriter().consume(byte_sent);
        }

        // // UDP
        // if (context.is_readable(gateway.udp_socket.getFD())) {
        //     for (Packet &packet : gateway.udp_socket.readPackets()) {
        //         on_packet(packet, c.first);
        //     }
        // }
        // if (context.is_writable(gateway.udp_socket.getFD())) {
        //     gateway.udp_socket.getBufWriter().appendPackets(gateway.send_udp_queue);
        //     ssize_t byte_sent = gateway.udp_socket.send(gateway.udp_socket.getBufWriter().getBuffer());
        //     gateway.udp_socket.getBufWriter().consume(byte_sent);
        // }
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
    static client_id id = 1;

    if (context.is_readable(listenFd)) {
        Gateway gateway;
        gateway.tcp_socket = TCPSocket::accept(listenFd);
        gateway.udp_socket.udp_address = {};
        clients.insert({id, gateway});
        on_connect(id++);
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
