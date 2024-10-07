
#include <array>
#include <cstdint>
#include <vector>
#include "lib_net/Packet.hpp"
#include "net.hpp"

namespace net {
struct UDPSocket {
    int sock;
    struct sockaddr_in dest;
    std::array<uint8_t, MAX_BUFFER_SIZE> recv_queue;

public:
    auto send(const std::vector<Packet> &packets) -> void
    {
        for (const auto &packet : packets) {
            sendto(
                sock, /*packet.data.data()*/, /*packet.data.size()*/, 0, (struct sockaddr *) &dest,
                sizeof(dest)
            );
        }
    }

    auto recv() -> std::vector<Packet>
    {
        std::vector<Packet> packets;
        constexpr auto len = sizeof(dest);
        auto bytes =
            recvfrom(sock, /*recv_queue.data()*/, /*recv_queue.size()*/, 0, (struct sockaddr *) &dest, &len);
        if (bytes <= 0) {
            return packets;
        }
        packets.emplace_back(recv_queue.data(), bytes);
        return packets;
    }
};
}
