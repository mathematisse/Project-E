#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <optional>
#include <utility>
#include <thread>

#include "Context.hpp"
#include "Gateway.hpp"
#include "lib_net/Packet.hpp"
#include "lib_net/TSQueue.hpp"

namespace net {
class Server {
public:
    bool host(std::uint16_t port);
    void start();
    void stop();
    void send_tcp(client_id id, Packet::MsgType type, const std::vector<std::uint8_t> &data);
    void send_udp(client_id id, Packet::MsgType type, const std::vector<std::uint8_t> &data);
    void send_tcp(Packet::MsgType type, const std::vector<std::uint8_t> &data);
    void send_udp(Packet::MsgType type, const std::vector<std::uint8_t> &data);
    std::optional<std::reference_wrapper<Gateway>> get_gateway(client_id id);
    void processConnections();
    void update();

    virtual void on_tcp_connect(client_id id) = 0;
    virtual void on_tcp_disconnect(client_id id) = 0;
    virtual void on_udp_connect(client_id id) = 0;
    virtual void on_packet(const Packet &packet, client_id id) = 0;

protected:
    socket_t udpFd;
    socket_t listenFd;
    std::atomic<bool> stop_flag = true;
    std::map<client_id, Gateway> clients;
    Context context;
    std::thread context_thread;
    client_id next_client_id = 1;
    TSQueue<std::pair<client_id, Packet>> receive_queue;
    TSQueue<std::pair<client_id, Packet>> send_queue_tcp;
    TSQueue<std::pair<client_id, Packet>> send_queue_udp;

    bool host_tcp(std::uint16_t port);
    bool host_udp(std::uint16_t port);
    void handle_new_tcp_connections();
};
}