#pragma once

#include <string>

class AddrParseError {
private:
    std::string message_;

public:
    explicit AddrParseError(std::string message):
        message_(std::move(message))
    {
    }
    [[nodiscard]] auto message() const -> const std::string & { return message_; }
};