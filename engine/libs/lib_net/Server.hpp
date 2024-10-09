#pragma once

#include <map>
#include <optional>
#include "Context.hpp"
#include "Gateway.hpp"

namespace net {
class Server {
public:
    bool host(std::uint16_t port);
    void send_tcp(client_id id, Packet::MsgType type, const std::vector<std::uint8_t> &data);
    void send_udp(client_id id, Packet::MsgType type, const std::vector<std::uint8_t> &data);
    std::optional<Gateway *> get_gateway(client_id id);
    void update();

    virtual void on_connect(client_id id) = 0;
    virtual void on_disconnect(client_id id) = 0;
    virtual void on_packet(Packet &packet, client_id id) = 0;

private:
    socket_t udpFd;
    socket_t listenFd;
    std::map<client_id, Gateway> clients;
    Context context;

    bool host_tcp(std::uint16_t port);
    bool host_udp(std::uint16_t port);
    void handle_connections();
};
}