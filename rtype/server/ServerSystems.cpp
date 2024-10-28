/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include <cmath>
#include <limits>
#include "ServerSystems.hpp"
#include "RTypeServer.hpp"
#include "Archetypes.hpp"
#include "RTypePackets.hpp"

#define RED_CLI "\033[31m"
#define GREEN_CLI "\033[32m"
#define YELLOW_CLI "\033[33m"
#define RESET "\033[0m"

namespace ECS {
namespace S {
// SYSTEM

SpawnEnnemySystem::SpawnEnnemySystem(
    EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId,
    net::RTypeServer &server, size_t maxEnnemyCount
):
    AMonoSystem(false),
    entityManager(entityManager),
    networkManager(networkManager),
    server(server),
    _spriteId(spriteId),
    _maxEnnemyCount(maxEnnemyCount)
{
}

void SpawnEnnemySystem::_innerOperate(
    C::Position::Pool::Types &cposition, C::Type::Pool::Types &ctype
)
{
    auto [type] = ctype;

    if (type != GameEntityType::PLAYER) {
        return;
    }
    if (enemyCount >= _maxEnnemyCount) {
        return;
    }
    auto need_to_spawn = _maxEnnemyCount - enemyCount;
    if (need_to_spawn <= 0) {
        return;
    }
    auto [x, y] = cposition;

    for (auto &enemy : entityManager.createEntities<E::BaseEntity>(need_to_spawn)) {
        int rand = std::rand() % 5;
        if (rand == 2) {
            enemy.setHealth({5});
            enemy.setWeapon({WeaponType::BIG_SHOT});
            enemy.setSize({180, 120});
        } else {
            enemy.setHealth({2});
            enemy.setWeapon({WeaponType::BULLET});
            enemy.setSize({80, 80});
        }
        enemy.setVelocity({0.0F, 0.0F});
        enemy.setType({GameEntityType::ENEMY});
        enemy.setColor({255, 0, 0, 255});
        enemy.setRotation({90});

        float _x = x + 500.0F + (float) (std::rand() % (int) x + 1000);
        float _y = 100.0F + (float) (std::rand() % 800);

        enemy.setPosition({_x, _y});
        enemy.setCanShoot({true, (enemy.getWeaponVal() == WeaponType::BIG_SHOT) ? 3.0F : 1.5F, 0.0F}
        );

        auto _netId = networkManager.getnewNetID();

        enemy.setNetworkID({_netId});

        server.send_tcp(
            RTypePacketType::NEW_ENNEMY,
            net::Packet::serializeStruct(NewEnnemy {_x, _y, _netId, rand})
        );
    }

    enemyCount = 5;
}

DestroyEntitiesSystem::DestroyEntitiesSystem(
    EntityManager &entityManager, net::RTypeServer &server
):
    AStatusMonoSystem(false, C::ENT_NEEDS_DESTROY),
    entityManager(entityManager),
    server(server)
{
}

void DestroyEntitiesSystem::_statusOperate(
    C::ChunkPos::Pool::Types &cchunkpos, C::NetworkID::Pool::Types &cnetworkid
)
{
    auto [networkid] = cnetworkid;

    entityManager.destroyEntity(cchunkpos);
    if (networkid == 0) {
        return;
    }
    server.send_tcp(
        RTypePacketType::ENTITY_DESTROYED, net::Packet::serializeStruct(EntityDestroyed {networkid})
    );
}

ShootSystem::ShootSystem(
    EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId,
    net::RTypeServer &server
):
    AMonoSystem(false),
    entityManager(entityManager),
    networkManager(networkManager),
    server(server),
    _spriteId(spriteId)
{
}

void ShootSystem::_innerOperate(
    C::Position::Pool::Types &cposition, C::Type::Pool::Types &ctype,
    C::CanShoot::Pool::Types &canshoot, C::IsShooting::Pool::Types &cIsShooting,
    C::Weapon::Pool::Types &cweapon
)
{
    auto [type] = ctype;
    auto [canShoot, base_delay, delay] = canshoot;
    auto [isShooting] = cIsShooting;
    if (canShoot == 0) {
        return;
    }
    if (delay > 0) {
        delay -= _deltaTime;
        return;
    }
    auto [weapon] = cweapon;
    if (((isShooting == 0) || type != GameEntityType::PLAYER) && type != GameEntityType::ENEMY) {
        return;
    }
    auto [x, y] = cposition;
    Vector2 playerPosition = {x, y};
    if (type == GameEntityType::ENEMY) {
        auto [enemyX, enemyY] = cposition;
        float closestDistance = std::numeric_limits<float>::max();
        float closestPlayerX = 0;
        float closestPlayerY = 0;
        if (playersPos.empty()) {
            return;
        }
        for (const auto &playerPos : playersPos) {
            float playerX = playerPos.x;
            float playerY = playerPos.y;
            auto distance =
                (float) std::sqrt(std::pow(playerX - enemyX, 2) + std::pow(playerY - enemyY, 2));
            if (distance < closestDistance) {
                closestDistance = distance;
                closestPlayerX = playerX;
                closestPlayerY = playerY;
            }
        }
        playerPosition = {closestPlayerX, closestPlayerY};
    }
    if (type == GameEntityType::ENEMY &&
        ((x - playerPosition.x) > 1000 || x <= playerPosition.x || y < playerPosition.y ||
         y > playerPosition.y + 50)) {
        return;
    }
    delay = base_delay;
    auto bullet = entityManager.createEntity<E::BaseEntity>();

    bool playerShot = type == GameEntityType::PLAYER;
    float factor = playerShot ? 1.0F : -1.0F;
    float _x = playerShot ? x + 80 + 35 : x - 35;

    bullet.setVelocity({300.0F * factor, 0.0F});
    bullet.setPosition({_x, y + 25});
    bullet.setRotation({90});
    bullet.setType({playerShot ? GameEntityType::BULLET : GameEntityType::BULLET_ENNEMY});
    bullet.setColor({255, 255, 0, 255});
    bullet.setCanShoot({false, 0.0F, 0.0F});

    if (weapon == WeaponType::BULLET) {
        bullet.setHealth({1});
        bullet.setSize({30, 30});
    }
    if (weapon == WeaponType::BIG_SHOT) {
        bullet.setHealth({5});
        bullet.setSize({70, 70});
    }

    auto _netId = networkManager.getnewNetID();
    bullet.setNetworkID({_netId});
    server.send_tcp(
        RTypePacketType::BULLET_SHOT,
        net::Packet::serializeStruct(
            BulletShot {_x, y + 25, playerShot, _netId, weapon == WeaponType::BIG_SHOT}
        )
    );
}

CountEnnemyAliveSystem::CountEnnemyAliveSystem(size_t &enemyCount):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    enemyCount(enemyCount)
{
}

void CountEnnemyAliveSystem::_statusOperate(C::Type::Pool::Types &ctype)
{
    auto [type] = ctype;
    if (type == GameEntityType::ENEMY) {
        enemyCount++;
    }
}

SendAllDataToNewClients::SendAllDataToNewClients():
    AStatusMonoSystem(false, C::ENT_ALIVE)
{
}

void SendAllDataToNewClients::_statusOperate(
    C::Position::Pool::Types &cposition, C::Velocity::Pool::Types &cvelocity,
    C::Type::Pool::Types &ctype, C::Health::Pool::Types &chealth,
    C::NetworkID::Pool::Types &cnetworkid
)
{
    auto [x, y] = cposition;
    auto [vX, vY] = cvelocity;
    auto [type] = ctype;
    auto [health] = chealth;
    auto [netId] = cnetworkid;

    // can put back to implement palyers joining in the middle of the game
    // this allows to send all the data to the new clients
    // if (type == GameEntityType::ENEMY) {
    //     for (auto &client : newClients) {
    //         server->send_tcp(
    //             client, RTypePacketType::NEW_ENNEMY, net::Packet::serializeStruct(NewEnnemy {x,
    //             y, netId})
    //         );
    //     }
    // } else if (type == GameEntityType::BULLET) {
    //     for (auto &client : newClients) {
    //         server->send_tcp(
    //             client, RTypePacketType::BULLET_SHOT,
    //             net::Packet::serializeStruct(BulletShot {x, y, true, netId})
    //         );
    //     }
    // } else if (type == GameEntityType::BULLET_ENNEMY) {
    //     for (auto &client : newClients) {
    //         server->send_tcp(
    //             client, RTypePacketType::BULLET_SHOT,
    //             net::Packet::serializeStruct(BulletShot {x, y, false, netId})
    //         );
    //     }
    // } else
    if (type == GameEntityType::PLAYER) {
        server->send_tcp(
            RTypePacketType::PLAYER_STATE,
            net::Packet::serializeStruct(PlayerState {x, y, vX, vY, health, netId})
        );
    }
}

MovePlayersSystem::MovePlayersSystem():
    AStatusMonoSystem(false, C::ENT_ALIVE)
{
}

void MovePlayersSystem::_statusOperate(
    C::Position::Pool::Types &cposition, C::Velocity::Pool::Types &cvelocity,
    C::Type::Pool::Types &ctype, C::NetworkID::Pool::Types &cnetworkid,
    C::IsShooting::Pool::Types &cIsShooting
)
{
    auto [type] = ctype;
    if (type != GameEntityType::PLAYER) {
        return;
    }
    auto [x, y] = cposition;
    auto [vX, vY] = cvelocity;
    auto [netId] = cnetworkid;
    auto [isShooting] = cIsShooting;

    for (auto &playerState : playerStates) {
        auto [pState, pNetId] = playerState;
        if (pNetId == netId) {
            vX = pState.x * 300;
            vY = pState.y * 300;
            isShooting = static_cast<char>(static_cast<int>(pState.shoot) != 0);
        }
    }
}

} // namespace S
} // namespace ECS