#include <cstring>

#include "lib_net/net/_base.hpp"
#include "lib_net/io/Result.hpp"
#include "lib_net/net/Socket.hpp"
#include "lib_net/net/SocketAddr.hpp"
#include "lib_net/result/Result.hpp"

namespace lnet::net {

#ifdef __linux__
constexpr int flag = MSG_NOSIGNAL;
#else
constexpr int flag = 0;
#endif

auto create_socket(int domain, int type) -> io::Result<int>
{
    auto sockfd = socket(domain, type, 0);
    if (sockfd == INVALID_SOCKET) {
        return io::Result<int>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<int>::Success(sockfd);
}

auto setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
    -> io::Result<result::Void>
{
    if (::setsockopt(sockfd, level, optname, static_cast<const char *>(optval), optlen) ==
        SOCKET_ERROR) {
        close_socket(sockfd);
        return io::Result<result::Void>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<result::Void>::Success({});
}

auto bind_socket(int sockfd, const struct sockaddr *address, socklen_t address_len)
    -> io::Result<result::Void>
{
    if (::bind(sockfd, address, address_len) == SOCKET_ERROR) {
        close_socket(sockfd);
        return io::Result<result::Void>::Error(std::error_code(errno, std::system_category()));
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
        ipv4->sin_addr.s_addr = addr.ip().to_v4().to_uint32_t();
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
        #ifdef _WIN32
        std::memcpy(ipv6_segments.data(), ipv6->sin6_addr.u.Byte, sizeof(ipv6_segments));
        #else
        std::memcpy(
            ipv6_segments.data(), ipv6->sin6_addr.s6_addr16, sizeof(ipv6->sin6_addr.s6_addr16)
        );
        #endif
        Ipv6Addr ip(ipv6_segments);
        return {IpAddr(ip), ntohs(ipv6->sin6_port)};
    }
}

auto Socket::create(int domain, int type) -> io::Result<Socket>
{
    auto sockfd = socket(domain, type, 0);
    if (sockfd == INVALID_SOCKET) {
        return io::Result<Socket>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<Socket>::Success(Socket(sockfd));
}

auto Socket::create(const SocketAddr &addr, int type) -> io::Result<Socket>
{
    auto sockfd = socket(addr.is_ipv4() ? AF_INET : AF_INET6, type, 0);
    if (sockfd == INVALID_SOCKET) {
        return io::Result<Socket>::Error(std::error_code(errno, std::system_category()));
    }
    // bind to address
    auto [address, address_len] = initialize_address(addr);
    if (::bind(sockfd, reinterpret_cast<struct sockaddr *>(&address), address_len) ==
        SOCKET_ERROR) {
        close_socket(sockfd);
        return io::Result<Socket>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<Socket>::Success(Socket(sockfd));
}

auto Socket::connect(const SocketAddr &addr) const -> io::Result<result::Void>
{
    auto [address, address_len] = initialize_address(addr);
    if (::connect(sockfd, reinterpret_cast<struct sockaddr *>(&address), address_len) ==
        SOCKET_ERROR) {
        return io::Result<result::Void>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<result::Void>::Success({});
}

auto Socket::close() const -> io::Result<result::Void>
{
    if (close_socket(sockfd) == SOCKET_ERROR) {
        return io::Result<result::Void>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<result::Void>::Success({});
}

auto Socket::accept() const -> io::Result<std::pair<Socket, SocketAddr>>
{
    struct sockaddr_storage address { };
    socklen_t address_len = sizeof(address);
    auto new_sockfd = ::accept(sockfd, reinterpret_cast<struct sockaddr *>(&address), &address_len);
    if (new_sockfd == INVALID_SOCKET) {
        return io::Result<std::pair<Socket, SocketAddr>>::Error(
            std::error_code(errno, std::system_category())
        );
    }
    return io::Result<std::pair<Socket, SocketAddr>>::Success(
        {Socket(new_sockfd), address_from_sockaddr(address)}
    );
}

auto Socket::read(std::span<std::uint8_t> &buf) const -> io::Result<std::size_t>
{
    auto nread = ::recv(sockfd, reinterpret_cast<char *>(buf.data()), buf.size(), flag);
    if (nread == SOCKET_ERROR) {
        return io::Result<std::size_t>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<std::size_t>::Success(nread);
}

auto Socket::recv_from(std::span<std::uint8_t> &buf
) const -> io::Result<std::pair<std::size_t, SocketAddr>>
{
    struct sockaddr_storage address { };
    socklen_t address_len = sizeof(address);
    auto nread = ::recvfrom(
        sockfd, reinterpret_cast<char *>(buf.data()), buf.size(), flag,
        reinterpret_cast<struct sockaddr *>(&address), &address_len
    );
    if (nread == SOCKET_ERROR) {
        return io::Result<std::pair<std::size_t, SocketAddr>>::Error(
            std::error_code(errno, std::system_category())
        );
    }
    auto addr = address_from_sockaddr(address);
    return io::Result<std::pair<std::size_t, SocketAddr>>::Success({nread, addr});
}

auto Socket::write(const std::span<std::uint8_t> &buf) const -> io::Result<std::size_t>
{
    auto nwritten =
        ::send(sockfd, reinterpret_cast<const char *>(buf.data()), buf.size(), flag);
    if (nwritten == SOCKET_ERROR) {
        return io::Result<std::size_t>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<std::size_t>::Success(nwritten);
}

auto Socket::send_to(const std::span<std::uint8_t> &buf, const SocketAddr &addr) const
    -> io::Result<std::size_t>
{
    auto [address, address_len] = initialize_address(addr);
    auto nwritten = ::sendto(
        sockfd, reinterpret_cast<const char *>(buf.data()), buf.size(), flag,
        reinterpret_cast<struct sockaddr *>(&address), address_len
    );
    if (nwritten == SOCKET_ERROR) {
        return io::Result<std::size_t>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<std::size_t>::Success(nwritten);
}

auto Socket::shutdown() const -> io::Result<result::Void>
{
    if (::shutdown(sockfd, 2) == SOCKET_ERROR) {
        return io::Result<result::Void>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<result::Void>::Success({});
}

auto Socket::peer_addr() const -> io::Result<SocketAddr>
{
    struct sockaddr_storage address { };
    socklen_t address_len = sizeof(address);
    if (::getpeername(sockfd, reinterpret_cast<struct sockaddr *>(&address), &address_len) ==
        SOCKET_ERROR) {
        return io::Result<SocketAddr>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<SocketAddr>::Success(address_from_sockaddr(address));
}

auto Socket::local_addr() const -> io::Result<SocketAddr>
{
    struct sockaddr_storage address { };
    socklen_t address_len = sizeof(address);
    if (::getsockname(sockfd, reinterpret_cast<struct sockaddr *>(&address), &address_len) ==
        SOCKET_ERROR) {
        return io::Result<SocketAddr>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<SocketAddr>::Success(address_from_sockaddr(address));
}

auto Socket::set_reuse_addr(bool enable) -> io::Result<result::Void>
{
    int optval = enable ? 1 : 0;
    return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}
#ifdef _WIN32
auto Socket::set_nonblocking(bool enable) -> io::Result<result::Void>
{
    unsigned long mode = enable ? 1 : 0;
    if (ioctlsocket(sockfd, FIONBIO, &mode) != 0) {
        return io::Result<result::Void>::Error(
            std::error_code(WSAGetLastError(), std::system_category())
        );
    }
    return io::Result<result::Void>::Success({});
}
#else
#include <fcntl.h>
auto Socket::set_nonblocking(bool enable) -> io::Result<result::Void>
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        return io::Result<result::Void>::Error(std::error_code(errno, std::system_category()));
    }
    if (enable) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    if (fcntl(sockfd, F_SETFL, flags) == -1) {
        return io::Result<result::Void>::Error(std::error_code(errno, std::system_category()));
    }
    return io::Result<result::Void>::Success({});
}
#endif

} // namespace lnet::net