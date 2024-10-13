#pragma once

#include "lib_net/Buffer.hpp"
#include "lib_net/Packet.hpp"
#include "lib_net/TCPSocket.hpp"
#include "lib_net/UDPInfo.hpp"

namespace net {
struct Gateway {

    Gateway() { generateNumber(); }

    TCPSocket tcp_socket;
    UDPInfo udp_info {};
    std::uint64_t generated_number = 0;
    std::vector<Packet> send_tcp_queue;
    std::vector<Packet> send_udp_queue;

    void generateNumber() { generated_number = transformNumberFunction(std::rand()); }
    // used to modify the number like hashing for security
    static inline std::uint64_t transformNumberFunction(std::uint64_t number)
    {
        std::uint64_t result = 0;
        constexpr std::uint64_t VERSION = 1;

        constexpr std::uint64_t MAGIC_NUMBER = 0x12024528;
        constexpr std::uint64_t SHIFT_AMOUNT = 3;
        constexpr std::uint64_t MULTIPLIER = 8;
        constexpr std::uint64_t MASK = 0xFFFFFFFFFFFFFFFF;

        result = result ^ ((MAGIC_NUMBER << SHIFT_AMOUNT) + (VERSION * MULTIPLIER)) ^ (number & MASK);
        return result;
    }
};
}
