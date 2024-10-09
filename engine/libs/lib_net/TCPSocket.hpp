
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "lib_net/net.hpp"
#include "lib_net/Packet.hpp"
#include "lib_net/UDPSocket.hpp"

namespace net {
class TCPSocket {
public:
    bool create();
    [[nodiscard]] bool bind(const std::string &ipAddress, uint16_t port) const;

    [[nodiscard]] bool listen(int backlog = MAX_LISTEN_BACKLOG) const;
    [[nodiscard]] TCPSocket accept() const;
    [[nodiscard]] bool connect(const std::string &ipAddress, uint16_t port) const;
    [[nodiscard]] inline bool is_connected() const { return socket_fd != INVALID_SOCKET; };

    [[nodiscard]] ssize_t send(const std::vector<std::uint8_t> &data) const;
    ssize_t recv(std::vector<std::uint8_t> &buffer, size_t size) const;
    ssize_t recvToBuffer();
    std::optional<Packet> readPacket();
    std::vector<Packet> readPackets();
    [[nodiscard]] BufReader &getBufReader();
    [[nodiscard]] BufWriter &getBufWriter();

    [[nodiscard]] socket_t getFD() const;
    void close();

    static TCPSocket accept(socket_t listenFD);

private:
    net::socket_t socket_fd = INVALID_SOCKET;
    BufReader buf_reader;
    BufWriter buf_writer;
};
}