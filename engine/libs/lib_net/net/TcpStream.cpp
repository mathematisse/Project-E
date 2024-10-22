
#include "_base.hpp"
#include "lib_net/net/Socket.hpp"
#include "lib_net/net/TcpStream.hpp"

namespace lnet::net {

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

} // namespace lnet::io