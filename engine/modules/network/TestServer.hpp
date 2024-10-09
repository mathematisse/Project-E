#pragma once

#include "lib_net/Server.hpp"

namespace net {
class TestServer : public Server {
public:
    void on_connect(client_id id) override;
    void on_disconnect(client_id id) override;
    void on_packet(const Packet &packet, client_id id) override;
};
}