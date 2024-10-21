
#include "_base.hpp"
#include "lib_net/net/Socket.hpp"
#include "lib_net/net/TcpStream.hpp"

namespace net::net {

auto TcpStream::connect(const SocketAddr &addr) -> io::Result<TcpStream>
{
    auto sock = Socket::create(addr, SOCK_STREAM);
    if (sock) {
        auto connect_result = sock.value().connect(addr);
        if (connect_result) {
            return io::Result<TcpStream>::Success(TcpStream(sock.value()));
        }
        auto close_result = sock.value().close();
        if (close_result.isError()) {
            return io::Result<TcpStream>::Error(close_result.error());
        }
        return io::Result<TcpStream>::Error(connect_result.error());
    }
    return io::Result<TcpStream>::Error(sock.error());
}

auto TcpStream::peer_addr() const -> io::Result<SocketAddr>
{
    auto addr = _sock.peer_addr();
    if (addr) {
        return io::Result<SocketAddr>::Success(addr.value());
    }
    return io::Result<SocketAddr>::Error(addr.error());
}

auto TcpStream::local_addr() const -> io::Result<SocketAddr>
{
    auto addr = _sock.local_addr();
    if (addr) {
        return io::Result<SocketAddr>::Success(addr.value());
    }
    return io::Result<SocketAddr>::Error(addr.error());
}

} // namespace net::io