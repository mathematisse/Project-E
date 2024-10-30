#include <cstddef>
#include <optional>
#include <cstring>

#include "Packet.hpp"

std::vector<std::uint8_t> net::Packet::serialize() const
{
    std::vector<std::uint8_t> buffer(sizeof(Header) + data.size());

    std::memcpy(buffer.data(), &header, sizeof(Header));
    std::copy(data.begin(), data.end(), buffer.begin() + sizeof(Header));
    return buffer;
}

net::Packet net::Packet::deserialize(MsgType type, const std::vector<std::uint8_t> &data)
{
    Packet packet;
    packet.header.type = type;
    packet.header.size = data.size();
    packet.data = data;
    return packet;
}

std::optional<net::Packet> net::Packet::deserialize(const std::vector<std::uint8_t> &data)
{
    Packet packet;

    if (data.size() < sizeof(Header)) {
        return std::nullopt;
    }
    std::memcpy(&packet.header, data.data(), sizeof(Header));
    if (data.size() < sizeof(Header) + packet.header.size) {
        return std::nullopt;
    }
    packet.data.assign(
        reinterpret_cast<const std::uint8_t *>(data.data()) + sizeof(Header),
        reinterpret_cast<const std::uint8_t *>(data.data()) + sizeof(Header) + packet.header.size
    );
    return packet;
}

std::optional<net::Packet> net::Packet::deserialize(const std::span<std::uint8_t> &data)
{
    Packet packet;

    if (data.size() < sizeof(Header)) {
        return std::nullopt;
    }
    std::memcpy(&packet.header, data.data(), sizeof(Header));
    if (data.size() < sizeof(Header) + packet.header.size) {
        return std::nullopt;
    }
    packet.data.assign(
        reinterpret_cast<const std::uint8_t *>(data.data()) + sizeof(Header),
        reinterpret_cast<const std::uint8_t *>(data.data()) + sizeof(Header) + packet.header.size
    );
    return packet;
}

// std::optional<net::Packet> net::Packet::deserialize(const std::vector<std::uint8_t> &data)
// {
//     Packet packet;

//     if (data.size() < sizeof(Header)) {
//         return std::nullopt;
//     }
//     std::memcpy(&packet.header, data.data(), sizeof(Header));
//     if (data.size() < sizeof(Header) + packet.header.size) {
//         return std::nullopt;
//     }
//     packet.data.assign(
//         data.begin() + sizeof(Header), data.begin() + sizeof(Header) + packet.header.size
//     );
//     return packet;
// }
