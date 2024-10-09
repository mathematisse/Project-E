
#pragma once

#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <vector>

#include "lib_net/Packet.hpp"
#include "lib_net/net.hpp"
#include "lib_net/Buffer.hpp"

namespace net {
class UDPSocket {
public:
    UDPSocket() = default;
    // return true if create is successful

    bool create();
    // return true if bind is successful
    [[nodiscard]] inline bool is_connected() const { return socket_fd != INVALID_SOCKET; };
    [[nodiscard]] bool bind(const std::string &ip, uint16_t port) const;
    // return number of bytes sent
    ssize_t send(const std::vector<std::uint8_t> &data) const;
    ssize_t recvToBuffer();

    std::optional<Packet> readPacket();
    std::vector<Packet> readPackets();
    [[nodiscard]] BufReader &getBufReader();
    [[nodiscard]] BufWriter &getBufWriter();

    [[nodiscard]] socket_t getFD() const;
    void close();

private:
    ssize_t recv(std::vector<std::uint8_t> &buffer, size_t size);

public:
    socket_t socket_fd = INVALID_SOCKET;
    sockaddr_in udp_address; // Store the client's address for sending responses
    BufReader buf_reader; // Buffer reader for UDP
    BufWriter buf_writer;
};

}
