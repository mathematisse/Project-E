
#include "lib_net/net/IpAddr.hpp"
#include "lib_net/net/AddrParseError.hpp"

namespace net::net {

auto Ipv4Addr::parse_ascii(const std::string &str) -> result::Result<Ipv4Addr, AddrParseError>
{
    if (str.empty()) {
        return result::Result<Ipv4Addr, AddrParseError>::Error(AddrParseError::Kind::EmptyString);
    }

    std::array<uint8_t, 4> addr;
    size_t i = 0;
    size_t j = 0;
    while (i < str.size() && j < 4) {
        size_t start = i;
        while (i < str.size() && str[i] != '.') {
            i++;
        }
        if (i - start > 3) {
            return result::Result<Ipv4Addr, AddrParseError>::Error(
                AddrParseError::Kind::InvalidIpv4Address
            );
        }
        addr[j] = std::stoi(str.substr(start, i - start));
        i++;
        j++;
    }
    if (j != 4) {
        return result::Result<Ipv4Addr, AddrParseError>::Error(
            AddrParseError::Kind::InvalidIpv4Address
        );
    }
    return result::Result<Ipv4Addr, AddrParseError>::Success(Ipv4Addr(addr));
}

auto Ipv6Addr::parse_ascii(const std::string &str) -> result::Result<Ipv6Addr, AddrParseError>
{
    if (str.empty()) {
        return result::Result<Ipv6Addr, AddrParseError>::Error(AddrParseError::Kind::EmptyString);
    }

    std::array<uint16_t, 8> addr;
    size_t i = 0;
    size_t j = 0;
    while (i < str.size() && j < 8) {
        size_t start = i;
        while (i < str.size() && str[i] != ':') {
            i++;
        }
        if (i - start > 4) {
            return result::Result<Ipv6Addr, AddrParseError>::Error(
                AddrParseError::Kind::InvalidIpv6Address
            );
        }
        addr[j] = static_cast<uint16_t>(std::stoul(str.substr(start, i - start), nullptr, 16));
        i++;
        j++;
    }
    if (j != 8) {
        return result::Result<Ipv6Addr, AddrParseError>::Error(
            AddrParseError::Kind::InvalidIpv6Address
        );
    }
    return result::Result<Ipv6Addr, AddrParseError>::Success(Ipv6Addr(addr));
}

} // namespace net::net