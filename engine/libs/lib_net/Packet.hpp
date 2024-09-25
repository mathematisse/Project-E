#pragma once

#include <cstdint>
#include <cstring>
#include <ostream>
#include <vector>

namespace net {
using byte = uint8_t;

template<typename T>
struct Packet {
public:
    struct Header {
        T type;
        uint32_t size;
    };

    Header header;
    std::vector<byte> data;

    [[nodiscard]] constexpr std::size_t get_header_size() const { return sizeof(Header); }

    [[nodiscard]] std::size_t size() const { return data.size(); }

    friend std::ostream &operator<<(std::ostream &os, const Packet<T> &packet)
    {
        os << "Type:" << int(packet.header.type) << " Size:" << packet.header.size;
        return os;
    }

    template<typename DataType>
    friend Packet<T> &operator<<(Packet<T> &packet, const DataType &data)
    {
        static_assert(
            std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector"
        );

        size_t size = packet.data.size();
        packet.data.resize(packet.data.size() + sizeof(DataType));
        std::memcpy(packet.data.data() + size, &data, sizeof(DataType));
        packet.header.size = packet.size();
        return packet;
    }

    template<typename DataType>
    friend Packet<T> &operator>>(Packet<T> &packet, DataType &data)
    {
        static_assert(
            std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector"
        );

        size_t size = packet.data.size() - sizeof(DataType);
        std::memcpy(&data, packet.data.data() + size, sizeof(DataType));
        packet.data.resize(size);
        packet.header.size = packet.size();
        return packet;
    }
};
}
