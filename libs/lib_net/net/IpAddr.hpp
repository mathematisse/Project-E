
#pragma once

#include <array>
#include <string>
#include <variant>
#include <cstdint>
#include <numeric>

#include "lib_net/result/Result.hpp"
#include "lib_net/net/AddrParseError.hpp"

namespace lnet::net {

class Ipv4Addr {
public:
    std::array<uint8_t, 4> octets;

    explicit Ipv4Addr(const std::array<uint8_t, 4> &addr):
        octets(addr)
    {
    }
    explicit Ipv4Addr(uint32_t addr):
        octets(
            {static_cast<uint8_t>((addr >> 0) & 0xFF), static_cast<uint8_t>((addr >> 8) & 0xFF),
             static_cast<uint8_t>((addr >> 16) & 0xFF), static_cast<uint8_t>(addr >> 24 & 0xFF)}
        )
    {
    }

    [[nodiscard]] inline std::string to_string() const
    {
        return std::to_string(octets[0]) + "." + std::to_string(octets[1]) + "." +
            std::to_string(octets[2]) + "." + std::to_string(octets[3]);
    }

    // only parses the address part of the string
    [[nodiscard]] static auto parse_ascii(const std::string &str
    ) -> result::Result<Ipv4Addr, AddrParseError>;

    [[nodiscard]] inline bool is_unspecified() const
    {
        return octets == std::array<uint8_t, 4> {0, 0, 0, 0};
    }
    [[nodiscard]] inline bool is_loopback() const { return octets[0] == 127; }

    [[nodiscard]] inline uint32_t to_uint32_t() const
    {
        constexpr uint32_t SHIFT_BITS = sizeof(uint8_t) * 8;
        return std::accumulate(octets.begin(), octets.end(), 0U, [](uint32_t acc, uint8_t octet) {
            return (acc << SHIFT_BITS) | octet;
        });
    }

    friend bool operator==(const Ipv4Addr &lhs, const Ipv4Addr &rhs)
    {
        return lhs.to_uint32_t() == rhs.to_uint32_t();
    }

    friend bool operator!=(const Ipv4Addr &lhs, const Ipv4Addr &rhs) { return !(lhs == rhs); }
    friend bool operator<(const Ipv4Addr &lhs, const Ipv4Addr &rhs)
    {
        return lhs.to_uint32_t() < rhs.to_uint32_t();
    }
};

class Ipv6Addr {
public:
    std::array<uint16_t, 8> segments;

    explicit Ipv6Addr(const std::array<uint16_t, 8> &addr):
        segments(addr)
    {
    }
    explicit Ipv6Addr(const uint16_t addr[8]):
        segments({addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7]})
    {
    }
    static auto any() -> Ipv6Addr
    {
        return Ipv6Addr(std::array<uint16_t, 8> {0, 0, 0, 0, 0, 0, 0, 0});
    }

    [[nodiscard]] std::string to_string() const
    {
        std::string result;
        for (size_t i = 0; i < segments.size(); ++i) {
            result += (i == 0 ? "" : ":") + std::to_string(segments[i]);
        }
        return result;
    }

    // only parses the address part of the string
    [[nodiscard]] static auto parse_ascii(const std::string &str
    ) -> result::Result<Ipv6Addr, AddrParseError>;

    [[nodiscard]] inline bool is_unspecified() const
    {
        return segments == std::array<uint16_t, 8> {0, 0, 0, 0, 0, 0, 0, 0};
    }

    [[nodiscard]] inline bool is_loopback() const
    {
        return segments == std::array<uint16_t, 8> {0, 0, 0, 0, 0, 0, 0, 1};
    }

    friend bool operator==(const Ipv6Addr &lhs, const Ipv6Addr &rhs)
    {
        bool result = true;
        for (size_t i = 0; i < lhs.segments.size(); ++i) {
            if (lhs.segments[i] != rhs.segments[i]) {
                return false;
            }
        }
        return result;
    }
    friend bool operator!=(const Ipv6Addr &lhs, const Ipv6Addr &rhs) { return !(lhs == rhs); }
    friend bool operator<(const Ipv6Addr &lhs, const Ipv6Addr &rhs)
    {
        for (size_t i = 0; i < lhs.segments.size(); ++i) {
            if (lhs.segments[i] < rhs.segments[i]) {
                return true;
            } else if (lhs.segments[i] > rhs.segments[i]) {
                return false;
            }
        }
        return false;
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

    [[nodiscard]] inline std::string to_string() const
    {
        if (std::holds_alternative<Ipv4Addr>(addr)) {
            return std::get<Ipv4Addr>(addr).to_string();
        } else {
            return std::get<Ipv6Addr>(addr).to_string();
        }
    }

    // checks wich type of address is in the string and calls the appropriate parse function
    [[nodiscard]] static auto inline parse_ascii(const std::string &str
    ) -> result::Result<IpAddr, AddrParseError>
    {
        if (str.empty()) {
            return result::Result<IpAddr, AddrParseError>::Error(AddrParseError::Kind::EmptyString);
        }

        if (str.find(':') != std::string::npos) {
            return Ipv6Addr::parse_ascii(str).map([](const Ipv6Addr &addr) {
                return IpAddr(addr);
            });
        } else {
            return Ipv4Addr::parse_ascii(str).map([](const Ipv4Addr &addr) {
                return IpAddr(addr);
            });
        }
    }
};
}
// namespace lnet::net
