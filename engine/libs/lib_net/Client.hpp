
#pragma once

#include "lib_net/Context.hpp"
#include "lib_net/Gateway.hpp"

namespace net {
class Client {
public:
    virtual ~Client() = default;

    auto connect_tcp(const std::string &ip, uint16_t port) -> bool;
    auto connect_udp(const std::string &ip, uint16_t port) -> bool;
    auto send_tcp(const Packet &packet) -> bool;
    auto send_udp(const Packet &packet) -> bool;

    virtual auto on_packet(const Packet &packet) -> void = 0;

public:
    net::Gateway gateway;
    Context context {};
};
}
