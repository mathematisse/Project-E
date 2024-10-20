
#pragma once

#include <cstdint>
#include <variant>
#include <string>

#include "lib_net/result/Result.hpp"
#include "lib_net/net/IpAddr.hpp"
#include "lib_net/net/AddrParseError.hpp"
#include "lib_net/net/IpAddr.hpp"

namespace net::net {

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
};

class SocketAddr {
private:
    SocketAddr(SocketAddrV4 addr):
        addr_(addr)
    {
    }
    SocketAddr(SocketAddrV6 addr):
        addr_(addr)
    {
    }

public:
    SocketAddr(const IpAddr &ip, uint16_t port)
    {
        if (ip.is_ipv4()) {
            addr_ = SocketAddrV4 {ip.to_v4(), port};
        } else {
            addr_ = SocketAddrV6 {ip.to_v6(), port, 0, 0};
        }
    }

    static auto parse_ascii(const std::string &str) -> result::Result<SocketAddr, AddrParseError> {

    }

    [[nodiscard]] IpAddr ip() const { }

    void set_ip(const IpAddr &new_ip) { }

    uint16_t port() const { }
    void set_port(uint16_t new_port) { }
    bool is_ipv4() const { std::holds_alternative<SocketAddrV4>(addr_); }
    bool is_ipv6() const { std::holds_alternative<SocketAddrV6>(addr_); }

public:
    SocketAddr() = default;

private:
    std::variant<SocketAddrV4, SocketAddrV6> addr_ = SocketAddrV4 {Ipv4Addr {{0, 0, 0, 0}}, 0};
};
} // namespace net::net