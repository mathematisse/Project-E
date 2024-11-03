#pragma once

#include <cstddef>
#include <span>

#include "lib_net/io/Result.hpp"
#include "lib_net/net/Socket.hpp"
#include "lib_net/result/Result.hpp"
#include "lib_net/net/SocketAddr.hpp"

namespace lnet::net {

class UdpSocket final {
public:
    Socket _sock;

    ~UdpSocket() = default;

    [[nodiscard]]
    static auto bind(const SocketAddr &addr) -> io::Result<UdpSocket>;
    static auto any(int domain) -> io::Result<UdpSocket>;

    auto recv_from(std::span<std::uint8_t> &buf) const -> io::Result<std::pair<size_t, SocketAddr>>;

    [[nodiscard]] auto
    send_to(const std::span<std::uint8_t> &buf, const SocketAddr &addr) const -> io::Result<size_t>;

    [[nodiscard]] auto connect(const SocketAddr &addr) const -> io::Result<result::Void>;

    [[nodiscard]] auto send(const std::span<std::uint8_t> &buf) const -> io::Result<size_t>;

    auto recv(std::span<std::uint8_t> &buf) const -> io::Result<size_t>;

    [[nodiscard]] inline auto local_addr() const -> io::Result<SocketAddr>
    {
        return _sock.local_addr();
    }

    [[nodiscard]] inline auto get_fd() const -> Socket::Type { return _sock.sockfd; }

    [[nodiscard]] inline auto close() const -> io::Result<result::Void> { return _sock.close(); }

    inline auto set_nonblocking(bool enable) -> io::Result<result::Void>
    {
        return _sock.set_nonblocking(enable);
    }

private:
    explicit UdpSocket(Socket sockfd):
        _sock(sockfd)
    {
    }
};

} // namespace lnet::io