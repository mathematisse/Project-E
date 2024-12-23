
#pragma once

#include "lib_net/net/SocketAddr.hpp"
#include "lib_net/net/TcpStream.hpp"

namespace lnet::net {

class TcpListener final {
public:
    Socket _sock;

    static constexpr auto DEFAULT_MAX_LISTENERS = 128;

    ~TcpListener() = default;

    [[nodiscard]]
    static auto bind(const SocketAddr &addr) -> io::Result<TcpListener>;

    [[nodiscard]]
    auto accept() const -> io::Result<std::pair<TcpStream, SocketAddr>>;

    [[nodiscard]]
    auto local_addr() const -> io::Result<SocketAddr>;

    [[nodiscard]]
    inline auto get_fd() const -> int
    {
        return _sock.sockfd;
    }

    [[nodiscard]] inline auto close() const -> io::Result<result::Void> { return _sock.close(); }

private:
    explicit TcpListener(Socket sock):
        _sock(sock)
    {
    }
};

} // namespace lnet::net