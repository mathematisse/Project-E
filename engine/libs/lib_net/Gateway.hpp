#pragma once

#include "Packet.hpp"
#include "TCPSocket.hpp"
#include "UDPSocket.hpp"

namespace net {
struct Gateway {
    TCPSocket tcp_socket;
    UDPSocket udp_socket;
    std::uint64_t generated_number = 0;
    std::vector<Packet> send_queue;
};
}
