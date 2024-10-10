
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

#include "lib_net/net.hpp"
#include "lib_net/Packet.hpp"
#include "lib_net/Buffer.hpp"

namespace net {
class TCPSocket {
public:
public:
    bool create();
    [[nodiscard]] bool bind(const std::string &ipAddress, uint16_t port) const;

    [[nodiscard]] bool listen(int backlog = MAX_LISTEN_BACKLOG) const;
    [[nodiscard]] bool connect(const std::string &ipAddress, uint16_t port) const;
    [[nodiscard]] inline bool is_connected() const { return socket_fd != INVALID_SOCKET; };
    [[nodiscard]] inline bool is_same_address(const sockaddr_in &addr) const
    {
        return tcp_address.sin_addr.s_addr == addr.sin_addr.s_addr;
    }

private:
    void set_address(const sockaddr_in &addr) { tcp_address = addr; }

public:
    [[nodiscard]] ssize_t send(const std::vector<std::uint8_t> &data) const;
    ssize_t recv(std::vector<std::uint8_t> &buffer, size_t size) const;
    ssize_t recvToBuffer();
    std::optional<Packet> readPacket();
    std::vector<Packet> readPackets();
    [[nodiscard]] BufReader &getBufReader();
    [[nodiscard]] BufWriter &getBufWriter();

    [[nodiscard]] socket_t getFD() const;
    void close();

    static std::optional<TCPSocket> accept(socket_t listenFD);

private:
    net::socket_t socket_fd = INVALID_SOCKET;
    sockaddr_in tcp_address;
    BufReader buf_reader;
    BufWriter buf_writer;
};
}