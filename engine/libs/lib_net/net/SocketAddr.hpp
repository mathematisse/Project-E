
#pragma once

#include <cstdint>
#include <variant>
#include <string>

#include "lib_net/result/Result.hpp"
#include "lib_net/net/IpAddr.hpp"
#include "lib_net/net/AddrParseError.hpp"

namespace lnet::net {

class SocketAddrV4 {
public:
    Ipv4Addr ip;
    uint16_t port;

    SocketAddrV4(Ipv4Addr ip, uint16_t port):
        ip(ip),
        port(port)
    {
    }

    [[nodiscard]] std::string to_string() const
    {
        return ip.to_string() + ":" + std::to_string(port);
    }

    friend bool operator==(const SocketAddrV4 &lhs, const SocketAddrV4 &rhs)
    {
        return lhs.ip == rhs.ip && lhs.port == rhs.port;
    }

    friend bool operator<(const SocketAddrV4 &lhs, const SocketAddrV4 &rhs)
    {
        return lhs.ip < rhs.ip || (lhs.ip == rhs.ip && lhs.port < rhs.port);
    }
};

struct SocketAddrV6 {
public:
    Ipv6Addr ip;
    uint16_t port;
    std::uint32_t flowinfo;
    std::uint32_t scope_id;

    SocketAddrV6(Ipv6Addr ip, uint16_t port, std::uint32_t flowinfo, std::uint32_t scope_id):
        ip(ip),
        port(port),
        flowinfo(flowinfo),
        scope_id(scope_id)
    {
    }

    [[nodiscard]] std::string to_string() const
    {
        return "[" + ip.to_string() + "]:" + std::to_string(port);
    }

    friend bool operator==(const SocketAddrV6 &lhs, const SocketAddrV6 &rhs)
    {
        return lhs.ip == rhs.ip && lhs.port == rhs.port && lhs.flowinfo == rhs.flowinfo &&
            lhs.scope_id == rhs.scope_id;
    }

    friend bool operator<(const SocketAddrV6 &lhs, const SocketAddrV6 &rhs)
    {
        return lhs.ip < rhs.ip || (lhs.ip == rhs.ip && lhs.port < rhs.port) ||
            (lhs.ip == rhs.ip && lhs.port == rhs.port && lhs.flowinfo < rhs.flowinfo) ||
            (lhs.ip == rhs.ip && lhs.port == rhs.port && lhs.flowinfo == rhs.flowinfo &&
             lhs.scope_id < rhs.scope_id);
    }
};

class SocketAddr {
private:
    explicit SocketAddr(SocketAddrV4 addr):
        addr_(addr)
    {
    }
    explicit SocketAddr(SocketAddrV6 addr):
        addr_(addr)
    {
    }

public:
    SocketAddr() = default;

    SocketAddr(const Ipv4Addr &ip, uint16_t port):
        addr_(SocketAddrV4 {ip, port})
    {
    }
    SocketAddr(const Ipv6Addr &ip, uint16_t port, std::uint32_t flowinfo, std::uint32_t scope_id):
        addr_(SocketAddrV6 {ip, port, flowinfo, scope_id})
    {
    }

    SocketAddr(const IpAddr &ip, uint16_t port)
    {
        if (ip.is_ipv4()) {
            addr_ = SocketAddrV4 {ip.to_v4(), port};
        } else {
            addr_ = SocketAddrV6 {ip.to_v6(), port, 0, 0};
        }
    }

    static auto parse_ascii(const std::string &str) -> result::Result<SocketAddr, AddrParseError>;

    [[nodiscard]] inline IpAddr ip() const
    {
        if (is_ipv4()) {
            return IpAddr(std::get<SocketAddrV4>(addr_).ip);
        } else {
            return IpAddr(std::get<SocketAddrV6>(addr_).ip);
        }
    }

    inline void set_ip(const IpAddr &new_ip)
    {
        if (new_ip.is_ipv4()) {
            std::get<SocketAddrV4>(addr_).ip = new_ip.to_v4();
        } else {
            std::get<SocketAddrV6>(addr_).ip = new_ip.to_v6();
        }
    }

    [[nodiscard]] inline uint16_t port() const
    {
        if (is_ipv4()) {
            return std::get<SocketAddrV4>(addr_).port;
        } else {
            return std::get<SocketAddrV6>(addr_).port;
        }
    }

    inline void set_port(uint16_t new_port)
    {
        if (is_ipv4()) {
            std::get<SocketAddrV4>(addr_).port = new_port;
        } else {
            std::get<SocketAddrV6>(addr_).port = new_port;
        }
    }

    [[nodiscard]] inline std::string to_string() const
    {
        if (is_ipv4()) {
            return std::get<SocketAddrV4>(addr_).to_string();
        } else {
            return std::get<SocketAddrV6>(addr_).to_string();
        }
    }

    [[nodiscard]] inline bool is_ipv4() const
    {
        return std::holds_alternative<SocketAddrV4>(addr_);
    }
    [[nodiscard]] inline bool is_ipv6() const
    {
        return std::holds_alternative<SocketAddrV6>(addr_);
    }

    // comparison operators

    friend bool operator==(const SocketAddr &lhs, const SocketAddr &rhs)
    {
        if (lhs.is_ipv4() && rhs.is_ipv4()) {
            return std::get<SocketAddrV4>(lhs.addr_) == std::get<SocketAddrV4>(rhs.addr_);
        } else if (lhs.is_ipv6() && rhs.is_ipv6()) {
            return std::get<SocketAddrV6>(lhs.addr_) == std::get<SocketAddrV6>(rhs.addr_);
        }
        return false;
    }

    friend bool operator!=(const SocketAddr &lhs, const SocketAddr &rhs) { return !(lhs == rhs); }
    friend bool operator<(const SocketAddr &lhs, const SocketAddr &rhs)
    {
        if (lhs.is_ipv4() && rhs.is_ipv4()) {
            return std::get<SocketAddrV4>(lhs.addr_) < std::get<SocketAddrV4>(rhs.addr_);
        } else if (lhs.is_ipv6() && rhs.is_ipv6()) {
            return std::get<SocketAddrV6>(lhs.addr_) < std::get<SocketAddrV6>(rhs.addr_);
        }
        return false;
    }
    friend bool operator>(const SocketAddr &lhs, const SocketAddr &rhs) { return rhs < lhs; }
    friend bool operator<=(const SocketAddr &lhs, const SocketAddr &rhs) { return !(lhs > rhs); }
    friend bool operator>=(const SocketAddr &lhs, const SocketAddr &rhs) { return !(lhs < rhs); }

private:
    std::variant<SocketAddrV4, SocketAddrV6> addr_ = SocketAddrV4 {Ipv4Addr {{0, 0, 0, 0}}, 0};
};
} // namespace lnet::net