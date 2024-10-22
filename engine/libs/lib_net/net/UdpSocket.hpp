#pragma once

#include <cstddef>
#include <span>

#include "lib_net/io/Result.hpp"
#include "lib_net/net/Socket.hpp"
#include "lib_net/result/Result.hpp"
#include "lib_net/net/SocketAddr.hpp"

namespace lnet::net {

class UdpSocket {
public:
    Socket _sock;

public:
    ~UdpSocket();

    [[nodiscard]]
    static auto bind(const SocketAddr &addr) -> io::Result<UdpSocket>;

    auto recv_from(std::span<std::byte> &buf) const -> io::Result<std::pair<size_t, SocketAddr>>;

    [[nodiscard]] auto
    send_to(const std::span<std::byte> &buf, const SocketAddr &addr) const -> io::Result<size_t>;

    [[nodiscard]] auto connect(const SocketAddr &addr) const -> io::Result<result::Void>;

    [[nodiscard]] auto send(const std::span<std::byte> &buf) const -> io::Result<size_t>;

    auto recv(std::span<std::byte> &buf) const -> io::Result<size_t>;

private:
    explicit UdpSocket(Socket sockfd):
        _sock(sockfd)
    {
    }
};

} // namespace lnet::io