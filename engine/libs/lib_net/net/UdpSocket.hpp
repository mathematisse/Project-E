#pragma once

#include <cstddef>
#include <string>
#include <span>
#include <cstdint>

#include "lib_net/io/Result.hpp"
#include "lib_net/result/Result.hpp"
#include "lib_net/net/SockAddr.hpp"

#pragma region WorkInProgress

namespace net::net {

class UdpSocket {

public:
    static auto bind(const SocketAddr &addr) -> io::Result<UdpSocket>;

    auto recv_from(std::span<std::byte> &buf) const -> io::Result<std::pair<size_t, std::string>>;

    [[nodiscard]] auto
    send_to(const std::span<std::byte> &buf, const SocketAddr &addr) const -> io::Result<size_t>;

    [[nodiscard]] auto connect(const SocketAddr &addr) const -> io::Result<result::Void>;

    [[nodiscard]] auto send(const std::span<std::byte> &buf) const -> io::Result<size_t>;

    auto recv(std::span<std::byte> &buf) const -> io::Result<size_t>;

    [[nodiscard]] auto set_nonblocking(bool nonblocking) const -> io::Result<result::Void>;

private:
    explicit UdpSocket(int sockfd);
    int sockfd_;
};

} // namespace net::io