#pragma once

#include <map>
#include <optional>
#include "Context.hpp"
#include "Gateway.hpp"

namespace net {
class Server {
public:
    using client_id = std::uint32_t;

    bool host(std::uint16_t port);
    ssize_t send_tcp(client_id id, const std::vector<Packet> &packets);
    ssize_t send_udp(client_id id, const std::vector<Packet> &packets);
    std::optional<Gateway &> get_gateway(client_id id);

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
};
}