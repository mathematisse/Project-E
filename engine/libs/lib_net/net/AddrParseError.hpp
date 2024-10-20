#pragma once

#include <string>

class AddrParseError {
public:
    enum class AddrParseErrorKind {
        EmptyString,
        InvalidIpv4Address,
        InvalidIpv6Address,
        MissingPort,
        InvalidPort,
        InvalidIp,
    };



private:
    std::string message_;

public:
    explicit AddrParseError(std::string message):
        message_(std::move(message))
    {
    }
    [[nodiscard]] auto message() const -> const std::string & { return message_; }
};