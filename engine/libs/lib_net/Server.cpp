#include "Server.hpp"

bool net::Server::host(std::uint16_t port) { return host_tcp(port) && host_udp(port); }

ssize_t net::Server::send_tcp(client_id id, const std::vector<Packet> &packets)
{
    auto gateway = get_gateway(id);

    if (!gateway.has_value()) {
        return 0;
    }
    return gateway->tcp_socket.send(packets);
}

ssize_t net::Server::send_udp(client_id id, const std::vector<Packet> &packets)
{
    auto gateway = get_gateway(id);

    if (!gateway.has_value()) {
        return 0;
    }
    return gateway->udp_socket.send(packets);
}

std::optional<net::Gateway &> net::Server::get_gateway(client_id id)
{
    auto it = clients.find(id);

    if (it == clients.end()) {
        return std::nullopt;
    }
    return it->second;
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
