#include <array>
#include <cstdint>
#include <cstring>
#include <sys/socket.h>

#include "lib_net/net/UdpSocket.hpp"
#include "lib_net/net/IpAddr.hpp"
#include "lib_net/net/_base.hpp"
#include "lib_net/result/Result.hpp"

namespace net::net {

auto create_socket(int domain) -> io::Result<int>
{
    auto sockfd = socket(domain, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        return io::Result<int>::Error(errno);
    }
    return io::Result<int>::Success(sockfd);
}

auto bind_socket(int sockfd, const struct sockaddr *address, socklen_t address_len)
    -> io::Result<result::Void>
{
    if (::bind(sockfd, address, address_len) == SOCKET_ERROR) {
        close_socket(sockfd);
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success({});
}

auto initialize_address(const SocketAddr &addr) -> std::pair<struct sockaddr_storage, socklen_t>
{
    struct sockaddr_storage address { };
    socklen_t address_len {};
    if (addr.is_ipv4()) {
        auto *ipv4 = reinterpret_cast<struct sockaddr_in *>(&address);
        ipv4->sin_family = AF_INET;
        ipv4->sin_port = htons(addr.port());
        ipv4->sin_addr.s_addr = htonl(addr.ip().to_v4().to_uint32_t());
        address_len = sizeof(*ipv4);
    } else {
        auto *ipv6 = reinterpret_cast<struct sockaddr_in6 *>(&address);
        ipv6->sin6_family = AF_INET6;
        ipv6->sin6_port = htons(addr.port());
        std::memcpy(&ipv6->sin6_addr, addr.ip().to_v6().segments.data(), 16);
        address_len = sizeof(*ipv6);
    }
    return {address, address_len};
}

auto address_from_sockaddr(const struct sockaddr_storage &address) -> SocketAddr
{
    if (address.ss_family == AF_INET) {
        const auto *ipv4 = reinterpret_cast<const struct sockaddr_in *>(&address);
        Ipv4Addr ip(ipv4->sin_addr.s_addr);
        return {IpAddr(ip), ntohs(ipv4->sin_port)};
    } else {
        const auto *ipv6 = reinterpret_cast<const struct sockaddr_in6 *>(&address);
        std::array<uint16_t, 8> ipv6_segments {};
        std::memcpy(
            ipv6_segments.data(), ipv6->sin6_addr.s6_addr16, sizeof(ipv6->sin6_addr.s6_addr16)
        );
        Ipv6Addr ip(ipv6_segments);
        return {IpAddr(ip), ntohs(ipv6->sin6_port)};
    }
}

UdpSocket::~UdpSocket() { close_socket(sockfd_); }

auto UdpSocket::bind(const SocketAddr &addr) -> io::Result<UdpSocket>
{
    auto sockfd = create_socket(addr.is_ipv4() ? AF_INET : AF_INET6);
    if (!sockfd) {
        return io::Result<UdpSocket>::Error(sockfd.error());
    }
    auto [address, address_len] = initialize_address(addr);
    auto bind_result =
        bind_socket(sockfd.value(), reinterpret_cast<struct sockaddr *>(&address), address_len);
    if (!bind_result) {
        return io::Result<UdpSocket>::Error(bind_result.error());
    }
    return io::Result<UdpSocket>::Success(UdpSocket(sockfd.value()));
}

auto UdpSocket::recv_from(std::span<std::byte> &buf
) const -> io::Result<std::pair<size_t, SocketAddr>>
{
    struct sockaddr_storage address;
    socklen_t address_len = sizeof(address);
    auto nread = recvfrom(
        sockfd_, reinterpret_cast<char *>(buf.data()), buf.size(), 0,
        reinterpret_cast<struct sockaddr *>(&address), &address_len
    );
    if (nread != SOCKET_ERROR) {
        auto addr = address_from_sockaddr(address);
        return io::Result<std::pair<size_t, SocketAddr>>::Success({nread, addr});
    }
    return io::Result<std::pair<size_t, SocketAddr>>::Error(errno);
}

auto UdpSocket::send_to(const std::span<std::byte> &buf, const SocketAddr &addr) const
    -> io::Result<size_t>
{
    auto [address, address_len] = initialize_address(addr);
    auto nwritten = sendto(
        sockfd_, reinterpret_cast<const char *>(buf.data()), buf.size(), 0,
        reinterpret_cast<struct sockaddr *>(&address), address_len
    );
    if (nwritten == SOCKET_ERROR) {
        return io::Result<size_t>::Error(errno);
    }
    return io::Result<size_t>::Success(nwritten);
}

auto UdpSocket::connect(const SocketAddr &addr) const -> io::Result<result::Void>
{
    auto [address, address_len] = initialize_address(addr);
    if (::connect(sockfd_, reinterpret_cast<struct sockaddr *>(&address), address_len) ==
        SOCKET_ERROR) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success({});
}

auto UdpSocket::send(const std::span<std::byte> &buf) const -> io::Result<size_t>
{
    auto nwritten = ::send(sockfd_, reinterpret_cast<const char *>(buf.data()), buf.size(), 0);
    if (nwritten == SOCKET_ERROR) {
        return io::Result<size_t>::Error(errno);
    }
    return io::Result<size_t>::Success(nwritten);
}

auto UdpSocket::recv(std::span<std::byte> &buf) const -> io::Result<size_t>
{
    auto nread = ::recv(sockfd_, reinterpret_cast<char *>(buf.data()), buf.size(), 0);
    if (nread == SOCKET_ERROR) {
        return io::Result<size_t>::Error(errno);
    }
    return io::Result<size_t>::Success(nread);
}

}
