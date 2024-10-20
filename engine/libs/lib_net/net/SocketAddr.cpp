
#include "lib_net/net/SocketAddr.hpp"

namespace net::net {

auto SocketAddr::parse_ascii(const std::string &str) -> result::Result<SocketAddr, AddrParseError>
{
    if (str.empty()) {
        return result::Result<SocketAddr, AddrParseError>::Error(AddrParseError::Kind::EmptyString);
    }

    // get last ':' character in the string (the port separator)
    auto colon = str.find_last_of(':');
    if (colon == std::string::npos) {
        return result::Result<SocketAddr, AddrParseError>::Error(AddrParseError::Kind::MissingPort);
    }

    // get the first part of the string (the IP address)
    auto ip = IpAddr::parse_ascii(str.substr(0, colon));
    if (ip.isError()) {
        return result::Result<SocketAddr, AddrParseError>::Error(ip.error());
    }

    // get the port number
    auto port = std::stoi(str.substr(colon + 1));
    if (port < 0 || port > 65535) {
        return result::Result<SocketAddr, AddrParseError>::Error(AddrParseError::Kind::InvalidPort);
    }
    return result::Result<SocketAddr, AddrParseError>::Success(
        SocketAddr {ip.value(), static_cast<uint16_t>(port)}
    );
}

} // namespace net::net