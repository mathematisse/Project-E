
#pragma once

#include "lib_net/net/SocketAddr.hpp"
#include "lib_net/io/Result.hpp"
#include <span>

namespace net::net {

/// Provides a convenient interface for working with sockets.
/// This class is a wrapper around windows & posix socket APIs.
/// Calling thoses functions directly is not recommended, use UdpSocket or TcpStream/Listener
/// instead.
class Socket {
protected:
    int sockfd;

public:
    static auto create(const SocketAddr &addr, int type) -> io::Result<Socket>;
    [[nodiscard]] auto connect(const SocketAddr &addr) const -> io::Result<result::Void>;
    [[nodiscard]] auto close() const -> io::Result<result::Void>;
    [[nodiscard]] auto accept() const -> io::Result<Socket>;
    [[nodiscard]] auto read(const std::span<std::byte> &buf) const -> io::Result<std::size_t>;
    [[nodiscard]] auto recv_from(const std::span<std::byte> &buf
    ) const -> io::Result<std::pair<std::size_t, SocketAddr>>;
    [[nodiscard]] auto write(const std::span<std::byte> &buf) const -> io::Result<std::size_t>;
    [[nodiscard]] auto send_to(const std::span<std::byte> &buf, const SocketAddr &addr) const
        -> io::Result<std::size_t>;

    [[nodiscard]] auto shutdown() const -> io::Result<result::Void>;

    [[nodiscard]] auto peer_addr() const -> io::Result<SocketAddr>;
    [[nodiscard]] auto local_addr() const -> io::Result<SocketAddr>;

private:
    explicit Socket(int sockfd);
};

} // namespace net::net