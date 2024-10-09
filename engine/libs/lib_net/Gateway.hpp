#pragma once

#include "lib_net/Packet.hpp"
#include "lib_net/TCPSocket.hpp"
#include "lib_net/UDPSocket.hpp"

namespace net {
struct Gateway {
    TCPSocket tcp_socket;
    UDPSocket udp_socket;
    std::uint64_t generated_number = 0;
    std::vector<Packet> send_tcp_queue;
    std::vector<Packet> send_udp_queue;
};
}
