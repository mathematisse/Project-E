#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace net {

struct Packet {
public:
    // from uint32 0 to 255 are reserved for system messages
    using MsgType = std::uint32_t;
    enum SystemTypes : Packet::MsgType {
        // server sends generated number to client (using TCP)
        ASKUDP_NUMBER = 1,
        // client responds with the transformed number (using UDP)
        ASKUDP_RESPONSE = 2,
        PING = 3,
        PONG = 4,
    };

    struct Header {
        MsgType type;
        std::uint16_t size;
    };

    Header header;
    std::vector<std::uint8_t> data;

    std::vector<std::uint8_t> serialize() const;
    static Packet deserialize(MsgType type, const std::vector<std::uint8_t> &data);
    static std::optional<Packet> deserialize(const std::vector<std::uint8_t> &data);
};

}
