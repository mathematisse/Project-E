
#include "lib_net/net/UdpSocket.hpp"
#include "lib_net/net/_base.hpp"
#include <cstring>

namespace net::net {

auto UdpSocket::bind(const SocketAddr &addr) -> io::Result<UdpSocket>
{
    int domain;

    if (addr.is_ipv4()) {
        domain = AF_INET;
    } else {
        domain = AF_INET6;
    }
    auto sockfd = socket(domain, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        return io::Result<UdpSocket>::Error(errno);
    }

    if (addr.is_ipv4()) {
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = htons(addr.port());
        address.sin_addr.s_addr = htonl(addr.ip().to_v4().to_uint32_t());
        if (::bind(sockfd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) ==
            SOCKET_ERROR) {
            close_socket(sockfd);
            return io::Result<UdpSocket>::Error(errno);
        }
    } else {
        struct sockaddr_in6 address;
        address.sin6_family = AF_INET6;
        address.sin6_port = htons(addr.port());
        std::memcpy(&address.sin6_addr, addr.ip().to_v6().segments.data(), 16);
        if (::bind(sockfd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) ==
            SOCKET_ERROR) {
            close_socket(sockfd);
            return io::Result<UdpSocket>::Error(errno);
        }
    }
    return io::Result<UdpSocket>::Success(UdpSocket(sockfd));
}

}
