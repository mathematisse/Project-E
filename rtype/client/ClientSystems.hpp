/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Components.hpp"
#include "NetworkManager.hpp"
#include "spatial2d/Components.hpp"
#include "RTypePackets.hpp"
#include "lib_ecs/Core/RawComponents.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "raylib.h"
#include "RTypeClient.hpp"

namespace ECS::S {

class MovePlayerSystem : public S::AMonoSystem<C::Velocity::Pool, C::Type::Pool> {
public:
    explicit MovePlayerSystem(net::RTypeClient &client, bool auto_shoot = false);
    ~MovePlayerSystem() override = default;

    net::RTypeClient &client;
    bool auto_shoot;
    size_t frame;

protected:
    void _innerOperate(
        typename C::Velocity::Pool::Types &cvelocity, typename C::Type::Pool::Types &ctype
    ) override;
};

class MoveOtherPlayerSystem : public S::AMonoSystem<
                                  C::Position::Pool, C::Velocity::Pool, C::Type::Pool,
                                  C::Health::Pool, C::NetworkID::Pool> {
public:
    explicit MoveOtherPlayerSystem();
    ~MoveOtherPlayerSystem() override = default;

    std::vector<ECS::PlayerState> playerStates;

protected:
    void _innerOperate(
        typename C::Position::Pool::Types &cposition, typename C::Velocity::Pool::Types &cvelocity,
        typename C::Type::Pool::Types &ctype, typename C::Health::Pool::Types &chealth,
        typename C::NetworkID::Pool::Types &cnetworkid
    ) override;
};

class DestroyEntitiesSystem : public S::AMonoSystem<C::ChunkPos::Pool, C::NetworkID::Pool> {
public:
    explicit DestroyEntitiesSystem(ECS::EntityManager &entityManager);
    ~DestroyEntitiesSystem() override = default;

    std::vector<ECS::EntityDestroyed> entitiesDestroyed;

protected:
    ECS::EntityManager &entityManager;
    void _innerOperate(
        typename C::ChunkPos::Pool::Types &cchunkPos, typename C::NetworkID::Pool::Types &cnetworkid
    ) override;
};

class UpdateEnginePosition
    : public S::AMonoSystem<C::EntityStatus::Pool, C::Position::Pool, C::Type::Pool> {
public:
    explicit UpdateEnginePosition();
    ~UpdateEnginePosition() override = default;

    Vector2 playerPosition = {0, 0};
    bool playerAlive = true;

protected:
    void _innerOperate(
        C::EntityStatus::Pool::Types &cstatus, C::Position::Pool::Types &cposition,
        C::Type::Pool::Types &ctype
    ) override;
};

} // namespace ECS::S
