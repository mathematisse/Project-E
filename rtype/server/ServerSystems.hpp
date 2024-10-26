/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "RTypePackets.hpp"
#include "RTypeServer.hpp"
#include "DecorEntities.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "GameEntities.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "NetworkManager.hpp"
#include "lib_net/Context.hpp"
#include "raylib.h"
#include <vector>

namespace ECS::S {

class SpawnEnnemySystem : public S::AStatusMonoSystem<C::PositionPool, C::TypePool> {
public:
    explicit SpawnEnnemySystem(
        EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId,
        net::RTypeServer &server, size_t maxEnnemyCount = 5
    );
    ~SpawnEnnemySystem() override = default;

    EntityManager &entityManager;
    NetworkManager &networkManager;
    size_t ennemyCount = 0;

    net::RTypeServer &server;

protected:
    size_t _spriteId = 0;
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::TypePool::Types &ctype
    ) override;

private:
    size_t _maxEnnemyCount = 0;
};

class DestroyEntitiesSystem : public S::AStatusMonoSystem<C::ChunkPosPool, C::NetworkIDPool> {
public:
    explicit DestroyEntitiesSystem(EntityManager &entityManager, net::RTypeServer &server);
    ~DestroyEntitiesSystem() override = default;

    EntityManager &entityManager;

    net::RTypeServer &server;

protected:
    void _statusOperate(
        typename C::ChunkPosPool::Types &cchunkpos, typename C::NetworkIDPool::Types &cnetworkid
    ) override;
};

class ShootSystem
    : public S::AStatusMonoSystem<
          C::PositionPool, C::TypePool, C::CanShootPool, C::IsShootingPool, C::WeaponPool>,
      public S::ADeltaTimeSystem {
public:
    explicit ShootSystem(
        EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId,
        net::RTypeServer &server
    );
    ~ShootSystem() override = default;

    EntityManager &entityManager;
    NetworkManager &networkManager;
    std::vector<Vector2> playersPos;

    net::RTypeServer &server;

protected:
    size_t _spriteId = 0;
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::TypePool::Types &ctype,
        typename C::CanShootPool::Types &canshoot, typename C::IsShootingPool::Types &cIsShooting,
        typename C::WeaponPool::Types &cweapon
    ) override;
};

class SendAllDataToNewClients
    : public S::AStatusMonoSystem<
          C::PositionPool, C::VelocityPool, C::TypePool, C::HealthPool, C::NetworkIDPool> {
public:
    explicit SendAllDataToNewClients();
    ~SendAllDataToNewClients() override = default;

    std::vector<net::client_id> newClients;
    net::RTypeServer *server = nullptr;

protected:
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::VelocityPool::Types &cvelocity,
        typename C::TypePool::Types &ctype, typename C::HealthPool::Types &chealth,
        typename C::NetworkIDPool::Types &cnetworkid
    ) override;
};

class CountEnnemyAliveSystem : public S::AStatusMonoSystem<C::TypePool> {
public:
    explicit CountEnnemyAliveSystem(size_t &ennemyCount);
    ~CountEnnemyAliveSystem() override = default;

    size_t ennemyCount = 0;

protected:
    void _statusOperate(C::TypePool::Types &ctype) override;
};

class MovePlayersSystem
    : public S::AStatusMonoSystem<
          C::PositionPool, C::VelocityPool, C::TypePool, C::NetworkIDPool, C::IsShootingPool> {
public:
    explicit MovePlayersSystem();
    ~MovePlayersSystem() override = default;

    std::vector<std::tuple<ECS::PlayerVelocityInput, size_t>> playerStates;

protected:
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::VelocityPool::Types &cvelocity,
        typename C::TypePool::Types &ctype, typename C::NetworkIDPool::Types &cnetworkid,
        typename C::IsShootingPool::Types &cIsShooting
    ) override;
};

} // namespace ECS::S
