#include <cstring>
#include "Packet.hpp"

std::vector<std::uint8_t> net::Packet::serialize()
{
    std::vector<std::uint8_t> buffer(sizeof(Header) + data.size());

    std::memcpy(buffer.data(), &header, sizeof(Header));
    std::memcpy(buffer.data() + sizeof(Header), data.data(), data.size());
    return buffer;
}

net::Packet net::Packet::deserialize(std::uint32_t type, const std::vector<std::uint8_t> &data)
{
    Packet packet;

    packet.header.type = type;
    packet.header.size = data.size();
    packet.data = data;
    return packet;
}
