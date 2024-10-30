/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "RTypePackets.hpp"
#include "RTypeServer.hpp"
#include "DecorSquare.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "Square.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "NetworkManager.hpp"
#include "lib_net/Context.hpp"
#include "raylib.h"
#include <cstddef>
#include <vector>

namespace ECS::S {

class SpawnEnnemySystem : public S::AStatusMonoSystem<C::PositionPool, C::TypePool> {
public:
    explicit SpawnEnnemySystem(
        EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId,
        net::RTypeServer &server, size_t maxEnnemyCount = 5
    );
    ~SpawnEnnemySystem() override = default;

    SpawnEnnemySystem(const SpawnEnnemySystem &other) = default;
    SpawnEnnemySystem(SpawnEnnemySystem &&other) = default;
    SpawnEnnemySystem &operator=(const SpawnEnnemySystem &other) = default;
    SpawnEnnemySystem &operator=(SpawnEnnemySystem &&other) = default;

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

    DestroyEntitiesSystem(const DestroyEntitiesSystem &other) = default;
    DestroyEntitiesSystem(DestroyEntitiesSystem &&other) = default;
    DestroyEntitiesSystem &operator=(const DestroyEntitiesSystem &other) = default;
    DestroyEntitiesSystem &operator=(DestroyEntitiesSystem &&other) = default;

    EntityManager &entityManager;

    net::RTypeServer &server;

protected:
    void _statusOperate(
        typename C::ChunkPosPool::Types &cchunkpos, typename C::NetworkIDPool::Types &cnetworkid
    ) override;
};

class ShootSystem
    : public S::AStatusMonoSystem<
          C::PositionPool, C::TypePool, C::CanShootPool, C::IsShootingPool, C::WeaponPool> {
public:
    explicit ShootSystem(
        EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId,
        net::RTypeServer &server
    );
    ~ShootSystem() override = default;

    ShootSystem(const ShootSystem &other) = default;
    ShootSystem(ShootSystem &&other) = default;
    ShootSystem &operator=(const ShootSystem &other) = default;
    ShootSystem &operator=(ShootSystem &&other) = default;

    EntityManager &entityManager;
    NetworkManager &networkManager;
    float deltaTime = 0.0f;
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

    SendAllDataToNewClients(const SendAllDataToNewClients &other) = default;
    SendAllDataToNewClients(SendAllDataToNewClients &&other) = default;
    SendAllDataToNewClients &operator=(const SendAllDataToNewClients &other) = default;
    SendAllDataToNewClients &operator=(SendAllDataToNewClients &&other) = default;

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

    CountEnnemyAliveSystem(const CountEnnemyAliveSystem &other) = default;
    CountEnnemyAliveSystem(CountEnnemyAliveSystem &&other) = default;
    CountEnnemyAliveSystem &operator=(const CountEnnemyAliveSystem &other) = default;
    CountEnnemyAliveSystem &operator=(CountEnnemyAliveSystem &&other) = default;

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

    MovePlayersSystem(const MovePlayersSystem &other) = default;
    MovePlayersSystem(MovePlayersSystem &&other) = default;
    MovePlayersSystem &operator=(const MovePlayersSystem &other) = default;
    MovePlayersSystem &operator=(MovePlayersSystem &&other) = default;

    std::vector<std::tuple<ECS::PlayerVelocityInput, size_t>> playerStates;

protected:
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::VelocityPool::Types &cvelocity,
        typename C::TypePool::Types &ctype, typename C::NetworkIDPool::Types &cnetworkid,
        typename C::IsShootingPool::Types &cIsShooting
    ) override;
};

class SpawnPowerUpSystem : public S::AStatusMonoSystem<C::PositionPool, C::TypePool> {
public:
    explicit SpawnPowerUpSystem(
        EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId,
        net::RTypeServer &server, size_t maxPowerUp = 1
    );
    ~SpawnPowerUpSystem() override = default;

    SpawnPowerUpSystem(const SpawnPowerUpSystem &other) = default;
    SpawnPowerUpSystem(SpawnPowerUpSystem &&other) = default;
    SpawnPowerUpSystem &operator=(const SpawnPowerUpSystem &other) = default;
    SpawnPowerUpSystem &operator=(SpawnPowerUpSystem &&other) = default;

    EntityManager &entityManager;
    NetworkManager &networkManager;
    size_t _powerUpCount = 0;
    net::RTypeServer &server;

protected:
    size_t _spriteId = 0;
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::TypePool::Types &ctype
    ) override;

private:
    size_t maxPowerUp = 0;
};

class CountPowerUpAliveSystem : public S::AStatusMonoSystem<C::TypePool> {
public:
    explicit CountPowerUpAliveSystem(size_t &powerUpCount);
    ~CountPowerUpAliveSystem() override = default;

    CountPowerUpAliveSystem(const CountPowerUpAliveSystem &other) = default;
    CountPowerUpAliveSystem(CountPowerUpAliveSystem &&other) = default;
    CountPowerUpAliveSystem &operator=(const CountPowerUpAliveSystem &other) = default;
    CountPowerUpAliveSystem &operator=(CountPowerUpAliveSystem &&other) = default;

    size_t powerUpCount = 0;

protected:
    void _statusOperate(C::TypePool::Types &ctype) override;
};

} // namespace ECS::S
