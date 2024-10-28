#pragma once

#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_net/Client.hpp"
#include "RTypePackets.hpp"
#include <cstddef>

namespace net {
class RTypeClient : public Client {
public:
    explicit RTypeClient(
        ECS::EntityManager &entityManager, std::vector<ECS::PlayerState> &playerStates,
        std::vector<ECS::EntityDestroyed> &entitiesDestroyed, float &cameraX
    );
    void on_packet(const Packet &packet) override;
    size_t ennemySpriteId = 0;
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
