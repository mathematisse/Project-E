#pragma once

#include <cstdint>
#include <vector>

namespace net {
struct Packet {
    struct Header {
        std::uint32_t type;
        std::uint16_t size;
    };

    Header header;
    std::vector<std::uint8_t> data;

    std::vector<std::uint8_t> serialize();
    static Packet deserialize(std::uint32_t type, const std::vector<std::uint8_t> &data);
};
}
