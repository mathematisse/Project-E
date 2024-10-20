
#pragma once

#include <array>
#include <string>
#include <variant>
#include <cstdint>

namespace net::net {

class Ipv4Addr {
public:
    std::array<uint8_t, 4> octets;

    explicit Ipv4Addr(const std::array<uint8_t, 4> &addr):
        octets(addr)
    {
    }

    [[nodiscard]] std::string to_string() const
    {
        return std::to_string(octets[0]) + "." + std::to_string(octets[1]) + "." +
            std::to_string(octets[2]) + "." + std::to_string(octets[3]);
    }

    bool is_unspecified() const { return octets == std::array<uint8_t, 4> {0, 0, 0, 0}; }
    bool is_loopback() const { return octets[0] == 127; }
};

class Ipv6Addr {
public:
    std::array<uint16_t, 8> segments;

    explicit Ipv6Addr(const std::array<uint16_t, 8> &addr):
        segments(addr)
    {
    }

    [[nodiscard]] std::string to_string() const
    {
        std::string result;
        for (size_t i = 0; i < segments.size(); ++i) {
            result += (i == 0 ? "" : ":") + std::to_string(segments[i]);
        }
        return result;
    }

    [[nodiscard]] inline bool is_unspecified() const
    {
        return segments == std::array<uint16_t, 8> {0, 0, 0, 0, 0, 0, 0, 0};
    }

    [[nodiscard]] inline bool is_loopback() const
    {
        return segments == std::array<uint16_t, 8> {0, 0, 0, 0, 0, 0, 0, 1};
    }
};

class IpAddr {
public:
    std::variant<Ipv4Addr, Ipv6Addr> addr;

    explicit IpAddr(const Ipv4Addr &ipv4):
        addr(ipv4)
    {
    }
    explicit IpAddr(const Ipv6Addr &ipv6):
        addr(ipv6)
    {
    }

    [[nodiscard]] inline bool is_unspecified() const
    {
        if (std::holds_alternative<Ipv4Addr>(addr)) {
            return std::get<Ipv4Addr>(addr).is_unspecified();
        } else {
            return std::get<Ipv6Addr>(addr).is_unspecified();
        }
    }

    [[nodiscard]] inline bool is_loopback() const
    {
        if (std::holds_alternative<Ipv4Addr>(addr)) {
            return std::get<Ipv4Addr>(addr).is_loopback();
        } else {
            return std::get<Ipv6Addr>(addr).is_loopback();
        }
    }

    [[nodiscard]] inline bool is_ipv4() const { return std::holds_alternative<Ipv4Addr>(addr); }
    [[nodiscard]] inline bool is_ipv6() const { return std::holds_alternative<Ipv6Addr>(addr); }

    [[nodiscard]] inline Ipv4Addr to_v4() const { return std::get<Ipv4Addr>(addr); }
    [[nodiscard]] inline Ipv6Addr to_v6() const { return std::get<Ipv6Addr>(addr); }

    [[nodiscard]] std::string to_string() const
    {
        if (std::holds_alternative<Ipv4Addr>(addr)) {
            return std::get<Ipv4Addr>(addr).to_string();
        } else {
            return std::get<Ipv6Addr>(addr).to_string();
        }
    }
};

}
// namespace net::net
