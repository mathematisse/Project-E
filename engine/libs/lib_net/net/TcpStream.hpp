#pragma once

#include <cstddef>
#include <span>

#include "lib_net/io/Read.hpp"
#include "lib_net/io/Result.hpp"
#include "lib_net/io/Write.hpp"
#include "lib_net/net/Socket.hpp"
#include "lib_net/result/Result.hpp"
#include "lib_net/net/SocketAddr.hpp"

#pragma region WorkInProgress

namespace net::net {

class TcpStream : public io::Write, public io::Read {
private:
    Socket _sock;

public:
    ~TcpStream() override = default;

    [[nodiscard]]
    static auto connect(const SocketAddr &addr) -> io::Result<TcpStream>;
    [[nodiscard]] auto peer_addr() const -> io::Result<SocketAddr>;
    [[nodiscard]] auto local_addr() const -> io::Result<SocketAddr>;
    [[nodiscard]] auto shutdown() const -> io::Result<result::Void>;

    inline auto read(const std::span<std::byte> &buf) -> io::Result<std::size_t> override
    {
        return _sock.read(buf);
    }
    inline auto write(const std::span<std::byte> &buf) -> io::Result<std::size_t> override
    {
        return _sock.write(buf);
    }
    inline auto flush() -> io::Result<result::Void> override
    {
        return io::Result<result::Void>::Success(result::Void {});
    }

protected:
    explicit TcpStream(Socket sock):
        _sock(sock)
    {
    }

    // TcpListener needs to be able to create TcpStream when accepting connections
    friend class TcpListener;
};

} // namespace net::io
