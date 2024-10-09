
#pragma once

#include <optional>
#include <vector>
#include <cstdint>

#include "Packet.hpp"

namespace net {

class BufReader {
public:
    void append(const std::vector<std::uint8_t> &data);
    std::optional<Packet> readPacket();
    [[nodiscard]] size_t size() const;

private:
    std::vector<std::uint8_t> buffer;
};

// Buffer Writer
class BufWriter {
public:
    void write(const std::vector<std::uint8_t> &data);
    void appendPacket(const Packet &packet);
    [[nodiscard]] std::vector<std::uint8_t> getBuffer() const;
    void consume(size_t size);
    void clear();
    std::vector<std::uint8_t> buffer;
};

} // namespace net