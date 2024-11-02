
#include "lib_net/net/IpAddr.hpp"
#include "lib_net/net/AddrParseError.hpp"
#include "lib_net/net/_base.hpp"

namespace lnet::net {

auto Ipv4Addr::parse_ascii(const std::string &str) -> result::Result<Ipv4Addr, AddrParseError>
{
    if (str.empty()) {
        return result::Result<Ipv4Addr, AddrParseError>::Error(AddrParseError::Kind::EmptyString);
    }

    addrinfo hints {};
    hints.ai_family = AF_INET;
    // we put 0 to get all the possible results
    hints.ai_socktype = 0;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE;

    addrinfo *result = nullptr;
    if (::getaddrinfo(str.c_str(), nullptr, &hints, &result) != 0) {
        return result::Result<Ipv4Addr, AddrParseError>::Error(
            AddrParseError::Kind::InvalidIpv4Address
        );
    }

    auto *ipv4 = reinterpret_cast<sockaddr_in *>(result->ai_addr);
    auto addr = Ipv4Addr(ipv4->sin_addr.s_addr);
    ::freeaddrinfo(result);
    return result::Result<Ipv4Addr, AddrParseError>::Success(addr);
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

} // namespace lnet::net