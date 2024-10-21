
#include "lib_net/net/TcpListener.hpp"
#include "lib_net/io/Result.hpp"
#include "lib_net/net/Socket.hpp"
#include "lib_net/net/SocketAddr.hpp"
#include "lib_net/net/TcpStream.hpp"
#include "lib_net/net/_base.hpp"

namespace net::net {

auto TcpListener::bind(const SocketAddr &addr) -> io::Result<TcpListener>
{

    auto sock = Socket::create(addr, SOCK_STREAM);
    if (!sock) {
        return io::Result<TcpListener>::Error(sock.error());
    }
#ifndef _WIN32
    sock.value().set_reuse_addr(true);
#endif
    auto listen_result = ::listen(sock.value().sockfd, DEFAULT_MAX_LISTENERS);
    if (listen_result == SOCKET_ERROR) {
        return io::Result<TcpListener>::Error(errno);
    }
    return io::Result<TcpListener>::Success(TcpListener(sock.value()));
}

auto TcpListener::accept() const -> io::Result<std::pair<TcpStream, SocketAddr>>
{
    auto sock_result = _sock.accept();
    if (!sock_result) {
        return io::Result<std::pair<TcpStream, SocketAddr>>::Error(sock_result.error());
    }
    auto [sock, addr] = sock_result.value();
    return io::Result<std::pair<TcpStream, SocketAddr>>::Success(
        std::make_pair(TcpStream(sock), addr)
    );
}

auto TcpListener::local_addr() const -> io::Result<SocketAddr> { return _sock.local_addr(); }

} // namespace net::net