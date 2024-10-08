#include "lib_net/UDPSocket.hpp"
#include <memory>

namespace net {

bool UDPSocket::create()
{
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    return socket_fd != INVALID_SOCKET;
}

// return true if bind is successful
[[nodiscard]] auto UDPSocket::bind(const std::string &ip, uint16_t port) const -> bool
{
    sockaddr_in addr {};
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    if (ip.empty()) {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    addr.sin_port = htons(port);

    return ::bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) == 0;
}

ssize_t UDPSocket::send(const std::vector<std::uint8_t> &data) const
{
    if (udp_address.sin_family == 0) {
        std::cerr << "Client address not set for UDP." << std::endl;
        return -1;
    }
    return ::sendto(
        socket_fd, reinterpret_cast<const char *>(data.data()), data.size(), 0,
        reinterpret_cast<const struct sockaddr *>(&udp_address), sizeof(udp_address)
    );
}

ssize_t UDPSocket::recv(std::vector<std::uint8_t> &buffer, size_t size)
{
    buffer.resize(size);
    socklen_t addr_len = sizeof(udp_address);
    return ::recvfrom(
        socket_fd, reinterpret_cast<char *>(buffer.data()), size, 0,
        reinterpret_cast<struct sockaddr *>(&udp_address), &addr_len
    );
}

ssize_t UDPSocket::recvToBuffer()
{
    std::vector<std::uint8_t> temp_buffer(MAX_BUFFER_SIZE); // Adjust size as needed
    ssize_t bytes_received = recv(temp_buffer, temp_buffer.size());
    if (bytes_received > 0) {
        buf_reader.append(temp_buffer);
    }
    return bytes_received;
}

auto UDPSocket::readPacket() -> std::optional<Packet> { return buf_reader.readPacket(); }

auto UDPSocket::readPackets() -> std::vector<Packet>
{
    std::vector<Packet> packets;
    while (auto packet = readPacket()) {
        packets.push_back(*packet);
    }
    return packets;
}

void UDPSocket::close()
{
    if (socket_fd != INVALID_SOCKET) {
        close_socket(socket_fd);
        socket_fd = INVALID_SOCKET;
    }
}

}
