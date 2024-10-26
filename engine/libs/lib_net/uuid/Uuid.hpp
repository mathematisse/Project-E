#pragma once

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <random>
#include <string>
#include <sstream>
#include <vector>

#include "lib_net/result/Result.hpp"

namespace lnet::uuid {

class UuidError {
public:
    enum class Type {
        InvalidByteStringSize,
        InvalidUuidString
    };

    explicit UuidError(Type type):
        type(type)
    {
    }

    [[nodiscard]]
    auto message() const -> std::string
    {
        switch (type) {
        case Type::InvalidByteStringSize:
            return "Invalid byte string size for UUID";
        case Type::InvalidUuidString:
            return "Invalid UUID string";
        }
    }

    Type type;
};

template<typename T>
using Result = result::Result<T, UuidError>;

class Uuid {
public:
    Uuid() = default;
    Uuid(uint64_t uuid):
        data(uuid)
    {
    }

    static auto new_from_bytes(const std::vector<std::byte> &bytes) -> Result<Uuid>
    {
        if (bytes.size() != sizeof(data)) {
            return Result<Uuid>::Error(UuidError {UuidError::Type::InvalidByteStringSize});
        }
        return Result<Uuid>::Success(Uuid {*reinterpret_cast<const uint64_t *>(bytes.data())});
    }

    static auto new_from_str(const std::string &s) -> Result<Uuid>
    {
        return new_from_str(s.c_str());
    }

    static auto new_from_str(const char *raw) -> Result<Uuid>
    {
        uint64_t uuid = 0;
        std::istringstream iss(raw);
        if (!(iss >> std::hex >> uuid)) {
            return Result<Uuid>::Error(UuidError {UuidError::Type::InvalidUuidString});
        }
        return Result<Uuid>::Success(Uuid {uuid});
    }

    auto from_str(const char *raw) -> Result<result::Void>
    {
        std::istringstream iss(raw);
        if (!(iss >> std::hex >> data)) {
            return Result<result::Void>::Error(UuidError {UuidError::Type::InvalidUuidString});
        }
        return Result<result::Void>::Success(result::Void {});
    }

    Uuid &operator=(const Uuid &other)
    {
        if (&other == this) {
            return *this;
        }
        data = other.data;
        return *this;
    }

    friend bool operator==(const Uuid &lhs, const Uuid &rhs) { return lhs.data == rhs.data; }
    friend bool operator<(const Uuid &lhs, const Uuid &rhs) { return lhs.data < rhs.data; }
    friend bool operator!=(const Uuid &lhs, const Uuid &rhs) { return !(lhs == rhs); }
    friend bool operator>(const Uuid &lhs, const Uuid &rhs) { return rhs < lhs; }
    friend bool operator<=(const Uuid &lhs, const Uuid &rhs) { return !(lhs > rhs); }
    friend bool operator>=(const Uuid &lhs, const Uuid &rhs) { return !(lhs < rhs); }

    [[nodiscard]]
    std::vector<std::byte> to_bytes() const
    {
        const std::byte *begin = reinterpret_cast<const std::byte *>(&data);
        const std::byte *end = begin + sizeof(data);
        return {begin, end};
    }

    [[nodiscard]]
    std::string to_str() const
    {
        std::ostringstream oss;
        oss << std::hex << data;
        return oss.str();
    }

    [[nodiscard]]
    size_t hash() const
    {
        return std::hash<uint64_t> {}(data);
    }

    friend std::ostream &operator<<(std::ostream &os, const Uuid &uuid)
    {
        os << uuid.to_str();
        return os;
    }

private:
    uint64_t data = 0;

    template<typename G>
        requires std::is_same_v<G, std::mt19937> || std::is_same_v<G, std::mt19937_64>
    friend class UuidGenerator;
};

}
