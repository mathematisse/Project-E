#pragma once

#include <functional>
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
<<<<<<< HEAD
=======
    void send_tcp(Packet::MsgType type, const std::vector<std::uint8_t> &data);
    void send_udp(Packet::MsgType type, const std::vector<std::uint8_t> &data);
>>>>>>> main
    std::optional<std::reference_wrapper<Gateway>> get_gateway(client_id id);
    void update();

    virtual void on_tcp_connect(client_id id) = 0;
    virtual void on_tcp_disconnect(client_id id) = 0;
    virtual void on_udp_connect(client_id id) = 0;
    virtual void on_packet(const Packet &packet, client_id id) = 0;

protected:
    socket_t udpFd;
    socket_t listenFd;
    std::map<client_id, Gateway> clients;
    Context context;
    client_id next_client_id = 1;

    bool host_tcp(std::uint16_t port);
    bool host_udp(std::uint16_t port);
    void handle_new_tcp_connections();
};
}