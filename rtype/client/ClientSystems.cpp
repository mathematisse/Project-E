/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "ClientSystems.hpp"
#include "DecorSquare.hpp"
#include "RTypePackets.hpp"
#include "Square.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include <iomanip>
#include <iostream>
#include "AssetsPath.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include <raylib.h>

#define RED_CLI "\033[31m"
#define GREEN_CLI "\033[32m"
#define YELLOW_CLI "\033[33m"
#define RESET "\033[0m"

namespace ECS {
namespace S {
// SYSTEM

MovePlayerSystem::MovePlayerSystem(net::RTypeClient &client):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    client(client)
{
}

void MovePlayerSystem::_statusOperate(C::VelocityPool::Types &cvelocity, C::TypePool::Types &ctype)
{
    auto [type] = ctype;
    if (type != SquareType::LPLAYER) {
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
    AStatusMonoSystem(false, C::ENT_ALIVE)
{
}

void MoveOtherPlayerSystem::_statusOperate(
    C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity, C::TypePool::Types &ctype,
    C::HealthPool::Types &chealth, C::NetworkIDPool::Types &cnetworkid
)
{
    auto [type] = ctype;
    if ((type != SquareType::PLAYER && type != SquareType::LPLAYER) || playerStates.empty()) {
        return;
    }
    auto [x, y] = cposition;
    auto [vX, vY, _] = cvelocity;
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
    AStatusMonoSystem(false, C::ENT_ALIVE),
    entityManager(entityManager)
{
}

void DestroyEntitiesSystem::_statusOperate(
    C::ChunkPosPool::Types &cchunkPos, C::NetworkIDPool::Types &cnetworkid
)
{
    auto [netId] = cnetworkid;

    for (auto &entityDestroyed : entitiesDestroyed) {
        if (entityDestroyed.netId == netId) {
            std::cout << RED_CLI << "Destroying entity with netId: " << netId << RESET << std::endl;
            entityManager.destroyEntity(cchunkPos);
        }
    }
}

UpdateEnginePosition::UpdateEnginePosition():
    AMonoSystem(false)
{
}

void UpdateEnginePosition::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::TypePool::Types &ctype
)
{
    auto [engine_status] = cstatus;
    auto [x, y] = cposition;
    auto [type] = ctype;
    if (type == SquareType::ENGINE) {
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