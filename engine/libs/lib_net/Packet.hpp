#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
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

    auto size() const -> std::size_t { return sizeof(header) + data.size(); }

    std::vector<std::uint8_t> serialize() const;

    template<typename T>
    static auto serializeStruct(const T &data) -> std::vector<std::uint8_t>
    {
        return std::vector<std::uint8_t>(
            reinterpret_cast<const std::uint8_t *>(&data),
            reinterpret_cast<const std::uint8_t *>(&data) + sizeof(T)
        );
    }

    static Packet deserialize(MsgType type, const std::vector<std::uint8_t> &data);
    static std::optional<Packet> deserialize(const std::vector<std::uint8_t> &data);
    // static std::optional<Packet> deserialize(const std::vector<std::uint8_t> &data);
    static std::optional<Packet> deserialize(const std::span<std::uint8_t> &data);

    template<typename T>
    static auto deserializeStruct(const std::vector<std::uint8_t> &data) -> std::optional<T>
    {
        if (data.size() != sizeof(T)) {
            return std::nullopt;
        }
        return *reinterpret_cast<const T *>(data.data());
    }

    // template<typename T>
    // static auto deserializeStruct(const std::vector<std::uint8_t> &data) -> std::optional<T>
    // {
    //     if (data.size() != sizeof(T)) {
    //         return std::nullopt;
    //     }
    //     return *reinterpret_cast<const T *>(data.data());
    // }
};

}
