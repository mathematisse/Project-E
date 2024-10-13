#pragma once

#include "lib_net/Client.hpp"

namespace net {
class TestClient : public Client {
public:
    void on_packet(const Packet &packet) override;
};
}
