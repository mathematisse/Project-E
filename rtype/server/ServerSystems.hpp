/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "RTypePackets.hpp"
#include "RTypeServer.hpp"
#include "Components.hpp"
#include "spatial2d/Components.hpp"
#include "lib_ecs/Core/PureComponents.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "NetworkManager.hpp"
#include "lib_net/Context.hpp"
#include "raylib.h"
#include <vector>

namespace ECS::S {

class SpawnEnnemySystem : public S::AMonoSystem<C::Position::Pool, C::Type::Pool> {
public:
    explicit SpawnEnnemySystem(
        EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId,
        net::RTypeServer &server, size_t maxEnnemyCount = 5
    );
    ~SpawnEnnemySystem() override = default;

    EntityManager &entityManager;
    NetworkManager &networkManager;
    size_t enemyCount = 0;

    net::RTypeServer &server;

protected:
    size_t _spriteId = 0;
    void _innerOperate(
        typename C::Position::Pool::Types &cposition, typename C::Type::Pool::Types &ctype
    ) override;

private:
    size_t _maxEnnemyCount = 0;
};

class DestroyEntitiesSystem : public S::AStatusMonoSystem<C::ChunkPos::Pool, C::NetworkID::Pool> {
public:
    explicit DestroyEntitiesSystem(EntityManager &entityManager, net::RTypeServer &server);
    ~DestroyEntitiesSystem() override = default;

    EntityManager &entityManager;

    net::RTypeServer &server;

protected:
    void _statusOperate(
        typename C::ChunkPos::Pool::Types &cchunkpos, typename C::NetworkID::Pool::Types &cnetworkid
    ) override;
};

class ShootSystem : public S::AMonoSystem<
                        C::Position::Pool, C::Type::Pool, C::CanShoot::Pool, C::IsShooting::Pool,
                        C::Weapon::Pool>,
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
    void _innerOperate(
        typename C::Position::Pool::Types &cposition, typename C::Type::Pool::Types &ctype,
        typename C::CanShoot::Pool::Types &canshoot,
        typename C::IsShooting::Pool::Types &cIsShooting, typename C::Weapon::Pool::Types &cweapon
    ) override;
};

class SendAllDataToNewClients : public S::AStatusMonoSystem<
                                    C::Position::Pool, C::Velocity::Pool, C::Type::Pool,
                                    C::Health::Pool, C::NetworkID::Pool> {
public:
    explicit SendAllDataToNewClients();
    ~SendAllDataToNewClients() override = default;

    std::vector<net::client_id> newClients;
    net::RTypeServer *server = nullptr;

protected:
    void _statusOperate(
        typename C::Position::Pool::Types &cposition, typename C::Velocity::Pool::Types &cvelocity,
        typename C::Type::Pool::Types &ctype, typename C::Health::Pool::Types &chealth,
        typename C::NetworkID::Pool::Types &cnetworkid
    ) override;
};

class CountEnnemyAliveSystem : public S::AStatusMonoSystem<C::Type::Pool> {
public:
    explicit CountEnnemyAliveSystem(size_t &enemyCount);
    ~CountEnnemyAliveSystem() override = default;

    size_t enemyCount = 0;

protected:
    void _statusOperate(C::Type::Pool::Types &ctype) override;
};

class MovePlayersSystem : public S::AStatusMonoSystem<
                              C::Position::Pool, C::Velocity::Pool, C::Type::Pool,
                              C::NetworkID::Pool, C::IsShooting::Pool> {
public:
    explicit MovePlayersSystem();
    ~MovePlayersSystem() override = default;

    std::vector<std::tuple<ECS::PlayerVelocityInput, size_t>> playerStates;

protected:
    void _statusOperate(
        typename C::Position::Pool::Types &cposition, typename C::Velocity::Pool::Types &cvelocity,
        typename C::Type::Pool::Types &ctype, typename C::NetworkID::Pool::Types &cnetworkid,
        typename C::IsShooting::Pool::Types &cIsShooting
    ) override;
};

} // namespace ECS::S
