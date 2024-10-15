
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
class UDPInfo {
public:
    UDPInfo() = default;

    bool is_identic(const sockaddr_in &addr) const
    {
        return udp_address.sin_addr.s_addr == addr.sin_addr.s_addr &&
            udp_address.sin_port == addr.sin_port;
    }

    std::optional<Packet> readPacket() { return buf_reader.readPacket(); }

    std::vector<Packet> readPackets()
    {
        std::vector<Packet> packets;

        while (auto packet = readPacket()) {
            packets.push_back(*packet);
        }
        return packets;
    }

public:
    sockaddr_in udp_address;
    BufReader buf_reader;
    BufWriter buf_writer;
};

}
