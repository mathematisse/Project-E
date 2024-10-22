#pragma once

#include "lib_net/Packet.hpp"
#include "lib_net/Context.hpp"
#include "lib_net/TCPSocket.hpp"
#include "lib_net/UDPInfo.hpp"
#include <string>
#include <lib_net/net.hpp>
#include <optional>
#include <vector>

namespace net {

class Client {
public:
    bool connect_tcp(const std::string &ip, uint16_t port);
    bool connect_udp(const std::string &ip, uint16_t port);
    bool is_connected() const { return gateway.tcp_socket.is_connected(); }
    void send_tcp(Packet::MsgType type, const std::vector<std::uint8_t> &data);
    void send_udp(Packet::MsgType type, const std::vector<std::uint8_t> &data);
    void update();

    virtual void on_packet(const Packet &packet) = 0;

protected:
    socket_t udpFd;
    Context context;
    Gateway gateway;
};

}