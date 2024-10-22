
#pragma once

#include "lib_net/net/SocketAddr.hpp"
#include "lib_net/net/TcpStream.hpp"

namespace net::net {

class TcpListener {
public:
    Socket _sock;

public:
    static constexpr auto DEFAULT_MAX_LISTENERS = 128;

    ~TcpListener();

    [[nodiscard]]
    static auto bind(const SocketAddr &addr) -> io::Result<TcpListener>;

    [[nodiscard]]
    auto accept() const -> io::Result<std::pair<TcpStream, SocketAddr>>;

    [[nodiscard]]
    auto local_addr() const -> io::Result<SocketAddr>;

private:
    explicit TcpListener(Socket sock):
        _sock(sock)
    {
    }
};

} // namespace net::net