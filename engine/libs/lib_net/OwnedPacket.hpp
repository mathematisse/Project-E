#pragma once

#include "Packet.hpp"
#include <memory>
#include <ostream>

namespace net {

enum class Ownership;

template<Ownership O, typename T>
class Connection;

template<Ownership O, typename T>
struct OwnedPacket {
    std::shared_ptr<Connection<O, T>> remote = nullptr;
    Packet<T> packet;

    friend std::ostream &operator<<(std::ostream &os, const OwnedPacket<O, T> &op)
    {
        os << op.packet;
        return os;
    }
};
}
