/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "DecorEntities.hpp"
#include "RTypePackets.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "GameEntities.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "raylib.h"
#include "RTypeClient.hpp"

namespace ECS::S {

class MovePlayerSystem : public S::AStatusMonoSystem<C::VelocityPool, C::TypePool> {
public:
    explicit MovePlayerSystem(net::RTypeClient &client);
    ~MovePlayerSystem() override = default;

    net::RTypeClient &client;
    size_t frame;

protected:
    void _statusOperate(
        typename C::VelocityPool::Types &cvelocity, typename C::TypePool::Types &ctype
    ) override;
};

class MoveOtherPlayerSystem
    : public S::AStatusMonoSystem<
          C::PositionPool, C::VelocityPool, C::TypePool, C::HealthPool, C::NetworkIDPool> {
public:
    explicit MoveOtherPlayerSystem();
    ~MoveOtherPlayerSystem() override = default;

    std::vector<ECS::PlayerState> playerStates;

protected:
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::VelocityPool::Types &cvelocity,
        typename C::TypePool::Types &ctype, typename C::HealthPool::Types &chealth,
        typename C::NetworkIDPool::Types &cnetworkid
    ) override;
};

class DestroyEntitiesSystem : public S::AStatusMonoSystem<C::ChunkPosPool, C::NetworkIDPool> {
public:
    explicit DestroyEntitiesSystem(ECS::EntityManager &entityManager);
    ~DestroyEntitiesSystem() override = default;

    std::vector<ECS::EntityDestroyed> entitiesDestroyed;

protected:
    ECS::EntityManager &entityManager;
    void _statusOperate(
        typename C::ChunkPosPool::Types &cchunkPos, typename C::NetworkIDPool::Types &cnetworkid
    ) override;
};

class UpdateEnginePosition
    : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool> {
public:
    explicit UpdateEnginePosition();
    ~UpdateEnginePosition() override = default;

    Vector2 playerPosition;
    char playerAlive;

protected:
    void _innerOperate(
        C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
        C::TypePool::Types &ctype
    ) override;
};

} // namespace ECS::S
