#pragma once

#include <cstdint>
#include <vector>

namespace net {
using byte = uint8_t;

template<typename T>
struct Packet {
    struct Header {
        T type;
        uint32_t size;
    };

    Header header;
    std::vector<byte> data;

    Packet(T type, std::vector<byte> data):
        header({type, static_cast<uint32_t>(data.size())}),
        data(data)
    {
    }
};
}
