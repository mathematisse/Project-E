#pragma once

#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/EntityManager.hpp"
// #include "lib_net/Client.hpp"
#include "lib_net/super/Server.hpp"
#include "RTypePackets.hpp"
#include <cstddef>
#include "lib_net/uuid/Uuid.hpp"

namespace net {
class RTypeClient : public net::Server {
public:
    using client_id = lnet::uuid::Uuid;
    explicit RTypeClient(
        ECS::EntityManager &entityManager, std::vector<ECS::PlayerState> &playerStates,
        std::vector<ECS::EntityDestroyed> &entitiesDestroyed, float &cameraX
    );
    void on_tcp_connect(client_id id) override {};
    void on_tcp_disconnect(client_id id) override {};
    void on_udp_connect(client_id id) override {};
    void on_packet(const Packet &packet, client_id server) override;
    size_t enemySpriteId = 0;
    size_t bulletSpriteId = 0;
    size_t bigShotSpriteId = 0;
    size_t frigateSpriteId = 0;
    size_t powerUpSpriteId = 0;
    ECS::Chunks::chunkPos_t playerPos;

    float &cameraX;
    bool started = false;

protected:
    ECS::EntityManager &_entityManager;
    std::vector<ECS::PlayerState> &playerStates;
    std::vector<ECS::EntityDestroyed> &entitiesDestroyed;
};
}
