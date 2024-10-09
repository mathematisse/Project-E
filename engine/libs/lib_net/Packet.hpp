#pragma once

#include <cstdint>
#include <vector>

namespace net {
struct Packet {
    using MsgType = std::uint32_t;

    struct Header {
        MsgType type;
        std::uint16_t size;
    };

    Header header;
    std::vector<std::uint8_t> data;

    std::vector<std::uint8_t> serialize() const;
    static Packet deserialize(MsgType type, const std::vector<std::uint8_t> &data);
};
}
