#pragma once

#include "RTypePackets.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_net/Context.hpp"
#include "lib_net/Server.hpp"
#include "NetworkManager.hpp"

namespace net {

class RTypeServer : public Server {
public:
    explicit RTypeServer(
        ECS::EntityManager &entityManager, NetworkManager &networkManager,
        std::vector<std::tuple<ECS::PlayerVelocityInput, size_t>> &playerStates,
        std::vector<client_id> &newClientIds
    );
    void on_tcp_connect(client_id id) override;
    void on_tcp_disconnect(client_id id) override;
    void on_udp_connect(client_id id) override;
    void on_packet(const Packet &packet, client_id id) override;
    [[nodiscard]] size_t clientCount() const;

    uint32_t pong_count = 0;

protected:
    ECS::EntityManager &_entityManager;
    NetworkManager &networkManager;

    std::map<client_id, size_t> client_netIds;
    std::vector<std::tuple<ECS::PlayerVelocityInput, size_t>> &playerStates;
    std::vector<client_id> &newClientIds;
};
}