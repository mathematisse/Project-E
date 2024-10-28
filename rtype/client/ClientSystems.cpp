/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "ClientSystems.hpp"
#include "RTypePackets.hpp"
#include "AssetsPath.hpp"
#include "lib_log/log.hpp"
#include <raylib.h>

namespace ECS {
namespace S {
// SYSTEM

MovePlayerSystem::MovePlayerSystem(net::RTypeClient &client, bool auto_shoot):
    AMonoSystem(false),
    client(client),
    auto_shoot(auto_shoot)
{
}

void MovePlayerSystem::_innerOperate(
    C::Velocity::Pool::Types &cvelocity, C::Type::Pool::Types &ctype
)
{
    auto [type] = ctype;
    if (type != GameEntityType::LPLAYER) {
        return;
    }

    float vX = 0;
    float vY = 0;
    if (IsKeyDown(KEY_UP)) {
        vY -= 1;
    }
    if (IsKeyDown(KEY_DOWN)) {
        vY += 1;
    }
    if (IsKeyDown(KEY_LEFT)) {
        vX -= 1;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        vX += 1;
    }
    if (!auto_shoot) {
        client.send_udp(
            ECS::RTypePacketType::PLAYER_VELOCITY,
            net::Packet::serializeStruct(ECS::PlayerVelocityInput {vX, vY, IsKeyDown(KEY_SPACE)})
        );
    } else {
        client.send_udp(
            ECS::RTypePacketType::PLAYER_VELOCITY,
            net::Packet::serializeStruct(ECS::PlayerVelocityInput {vX, vY, true})
        );
    }
}

MoveOtherPlayerSystem::MoveOtherPlayerSystem():
    AMonoSystem(false)
{
}

void MoveOtherPlayerSystem::_innerOperate(
    C::Position::Pool::Types &cposition, C::Velocity::Pool::Types &cvelocity,
    C::Type::Pool::Types &ctype, C::Health::Pool::Types &chealth,
    C::NetworkID::Pool::Types &cnetworkid
)
{
    auto [type] = ctype;
    if ((type != GameEntityType::PLAYER && type != GameEntityType::LPLAYER) ||
        playerStates.empty()) {
        return;
    }
    auto [x, y] = cposition;
    auto [vX, vY] = cvelocity;
    auto [health] = chealth;
    auto [netId] = cnetworkid;
    // print size of pstates
    for (auto it = playerStates.begin(); it != playerStates.end();) {
        if (it->netId == netId) {
            x = it->x;
            y = it->y;
            vX = it->vx;
            vY = it->vy;
            health = it->health;
            it = playerStates.erase(it);
            break;
        } else {
            ++it;
        }
    }
}

DestroyEntitiesSystem::DestroyEntitiesSystem(ECS::EntityManager &entityManager):
    AMonoSystem(false),
    entityManager(entityManager)
{
}

void DestroyEntitiesSystem::_innerOperate(
    C::ChunkPos::Pool::Types &cchunkPos, C::NetworkID::Pool::Types &cnetworkid
)
{
    auto [netId] = cnetworkid;

    for (auto &entityDestroyed : entitiesDestroyed) {
        if (entityDestroyed.netId == netId) {
            LOG_DEBUG(
                LOG_RED "Destroying entity with netId: " + std::to_string(netId) + LOG_COLOR_RESET
            );
            entityManager.destroyEntity(cchunkPos);
        }
    }
}

UpdateEnginePosition::UpdateEnginePosition():
    AMonoSystem(false)
{
}

void UpdateEnginePosition::_innerOperate(
    C::EntityStatus::Pool::Types &cstatus, C::Position::Pool::Types &cposition,
    C::Type::Pool::Types &ctype
)
{
    auto [engine_status] = cstatus;
    auto [x, y] = cposition;
    auto [type] = ctype;
    if (type == GameEntityType::ENGINE) {
        if (engine_status != C::EntityStatusEnum::ENT_ALIVE) {
            return;
        }
        if (playerAlive == 0) {
            engine_status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
            return;
        }
        x = playerPosition.x + 80;
        y = playerPosition.y;
    }
}

} // namespace S
} // namespace ECS