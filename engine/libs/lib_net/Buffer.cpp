#include <optional>
#include <vector>
#include <cstdint>

#include "lib_net/Buffer.hpp"

namespace net {

void BufReader::append(const std::vector<std::uint8_t> &data)
{
    buffer.insert(buffer.end(), data.begin(), data.end());
}

auto BufReader::readPacket() -> std::optional<Packet>
{
    if (buffer.size() < sizeof(Packet::Header)) {
        return std::nullopt;
    }
    // cast buffer to Header
    const auto *header = reinterpret_cast<const Packet::Header *>(buffer.data());
    if (buffer.size() < sizeof(Packet::Header) + header->size) {
        return std::nullopt;
    }
    // copy header and data from buffer, and then remove them from buffer
    Packet packet;
    packet.header = *header;
    packet.data.assign(
        buffer.begin() + sizeof(Packet::Header), buffer.begin() + sizeof(Packet::Header) + header->size
    );
    buffer.erase(buffer.begin(), buffer.begin() + sizeof(Packet::Header) + header->size);
    return packet;
}

size_t BufReader::size() const { return buffer.size(); }

void BufWriter::write(const std::vector<std::uint8_t> &data)
{
    buffer.insert(buffer.end(), data.begin(), data.end());
}

void BufWriter::appendPacket(const Packet &packet)
{
    const auto serialized = packet.serialize();
    write(serialized);
}

void BufWriter::appendPackets(const std::vector<Packet> &packets)
{
    for (const auto &packet : packets) {
        appendPacket(packet);
    }
}

std::vector<std::uint8_t> BufWriter::getBuffer() const { return buffer; }

void BufWriter::consume(size_t size)
{
    if (size > buffer.size()) {
        buffer.clear();
    } else {
        buffer.erase(buffer.begin(), buffer.begin() + size);
    }
}

void BufWriter::clear() { buffer.clear(); }

} // namespace net