#pragma once

#include <cstddef>
#include <span>

#include "lib_net/io/Read.hpp"
#include "lib_net/io/Result.hpp"
#include "lib_net/io/Write.hpp"
#include "lib_net/net/Socket.hpp"
#include "lib_net/result/Result.hpp"
#include "lib_net/net/SocketAddr.hpp"

namespace lnet::net {

class TcpStream final : public io::Write, public io::Read {
public:
    Socket _sock;

    ~TcpStream() override = default;

    [[nodiscard]]
    static auto connect(const SocketAddr &addr) -> io::Result<TcpStream>;
    [[nodiscard]]
    inline auto peer_addr() const -> io::Result<SocketAddr>
    {
        return _sock.peer_addr();
    }
    [[nodiscard]]
    inline auto local_addr() const -> io::Result<SocketAddr>
    {
        return _sock.local_addr();
    }
    [[nodiscard]]
    inline auto shutdown() const -> io::Result<result::Void>
    {
        return _sock.shutdown();
    }

    inline auto read(std::span<std::uint8_t> &buf) -> io::Result<std::size_t> override
    {
        return _sock.read(buf);
    }

    inline auto write(const std::span<std::uint8_t> &buf) -> io::Result<std::size_t> override
    {
        return _sock.write(buf);
    }
    inline auto flush() -> io::Result<result::Void> override
    {
        return io::Result<result::Void>::Success(result::Void {});
    }
    [[nodiscard]] inline auto close() const -> io::Result<result::Void> { return _sock.close(); }

    [[nodiscard]]
    inline auto get_fd() const -> Socket::Type
    {
        return _sock.sockfd;
    }
    inline auto set_nonblocking(bool enable) -> io::Result<result::Void>
    {
        return _sock.set_nonblocking(enable);
    }

protected:
    explicit TcpStream(Socket sock):
        _sock(sock)
    {
    }

    // TcpListener needs to be able to create TcpStream when accepting connections
    friend class TcpListener;
};

} // namespace lnet::io
