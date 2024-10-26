#include <cstring>
#include <sys/socket.h>

#include "lib_net/net/UdpSocket.hpp"
#include "lib_net/result/Result.hpp"

namespace lnet::net {

auto UdpSocket::bind(const SocketAddr &addr) -> io::Result<UdpSocket>
{
    auto sock = Socket::create(addr, SOCK_DGRAM);
    if (sock) {
        return io::Result<UdpSocket>::Success(UdpSocket(sock.value()));
    }
    return io::Result<UdpSocket>::Error(sock.error());
}

auto UdpSocket::any(int domain) -> io::Result<UdpSocket>
{
    auto sock = Socket::create(domain, SOCK_DGRAM);
    if (sock) {
        return io::Result<UdpSocket>::Success(UdpSocket(sock.value()));
    }
    return io::Result<UdpSocket>::Error(sock.error());
}

auto UdpSocket::recv_from(std::span<std::byte> &buf
) const -> io::Result<std::pair<size_t, SocketAddr>>
{
    return _sock.recv_from(buf);
}

auto UdpSocket::send_to(const std::span<std::byte> &buf, const SocketAddr &addr) const
    -> io::Result<size_t>
{
    return _sock.send_to(buf, addr);
}

auto UdpSocket::connect(const SocketAddr &addr) const -> io::Result<result::Void>
{
    return _sock.connect(addr);
}

auto UdpSocket::send(const std::span<std::byte> &buf) const -> io::Result<size_t>
{
    return _sock.write(buf);
}

auto UdpSocket::recv(std::span<std::byte> &buf) const -> io::Result<size_t>
{
    return _sock.read(buf);
}

}
