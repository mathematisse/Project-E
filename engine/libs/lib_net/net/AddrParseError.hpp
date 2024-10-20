#pragma once

#include <string>

class AddrParseError {
public:
    enum class Kind {
        EmptyString,
        InvalidIpv4Address,
        InvalidIpv6Address,
        MissingPort,
        InvalidPort,
        InvalidIp,
    };

private:
    Kind _kind;

public:
    explicit AddrParseError(Kind kind):
        _kind(kind)
    {
    }

    [[nodiscard]] auto message() const -> std::string
    {
        switch (_kind) {
        case Kind::EmptyString:
            return "Empty string provided";
        case Kind::InvalidIpv4Address:
            return "Invalid IPv4 address";
        case Kind::InvalidIpv6Address:
            return "Invalid IPv6 address";
        case Kind::MissingPort:
            return "Missing port";
        case Kind::InvalidPort:
            return "Invalid port";
        case Kind::InvalidIp:
            return "Invalid IP address";
        default:
            return "Unknown error";
        }
    }
};