#pragma once

#include "Packet.hpp"
#include <memory>

namespace net {

template<typename T>
class Connection;

template<typename T>
struct OwnedPacket {
    std::shared_ptr<Connection<T>> remote = nullptr;
    Packet<T> packet;
};
}
