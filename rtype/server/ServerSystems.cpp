/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include <cmath>
#include <limits>
#include "ServerSystems.hpp"
#include "DecorEntities.hpp"
#include "RTypeServer.hpp"
#include "GameEntities.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include <iostream>
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
    AStatusMonoSystem(false, C::ENT_ALIVE),
    entityManager(entityManager),
    networkManager(networkManager),
    server(server),
    _spriteId(spriteId),
    _maxEnnemyCount(maxEnnemyCount)
{
}

void SpawnEnnemySystem::_statusOperate(C::PositionPool::Types &cposition, C::TypePool::Types &ctype)
{
    auto [type] = ctype;

    if (type != GameEntityType::PLAYER) {
        return;
    }
    if (ennemyCount >= _maxEnnemyCount) {
        return;
    }
    auto need_to_spawn = _maxEnnemyCount - ennemyCount;
    if (need_to_spawn <= 0) {
        return;
    }
    auto [x, y] = cposition;
    auto ennemies = entityManager.createEntities("GameEntity", need_to_spawn, ECS::C::ENT_ALIVE);

    for (const auto &entity : ennemies) {
        auto ref = entityManager.getEntity(entity);

        auto *square_ennemy = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
        if (square_ennemy == nullptr) {
            std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
            return;
        }
        int rand = std::rand() % 5;
        if (rand == 2) {
            square_ennemy->getHealth()->set<0>(5);
            square_ennemy->getWeapon()->set<0>(WeaponType::BIG_SHOT);
            square_ennemy->getSize()->set<0>(180);
            square_ennemy->getSize()->set<1>(120);
        } else {
            square_ennemy->getHealth()->set<0>(2);
            square_ennemy->getWeapon()->set<0>(WeaponType::BULLET);
            square_ennemy->getSize()->set<0>(80);
            square_ennemy->getSize()->set<1>(80);
        }
        square_ennemy->getVelocity()->set<0>(0.0F);
        square_ennemy->getVelocity()->set<1>(0.0F);
        square_ennemy->getVelocity()->set<2>(100.0F);
        square_ennemy->getType()->set<0>(SquareType::ENEMY);
        square_ennemy->getColor()->set<0>(255);
        square_ennemy->getColor()->set<1>(0);
        square_ennemy->getColor()->set<2>(0);
        square_ennemy->getColor()->set<3>(255);
        square_ennemy->getRotation()->set<0>(90);

        float _x = x + 500 + std::rand() % (int) (x + 1000);
        float _y = 100 + std::rand() % 800;
        square_ennemy->getPosition()->set<0>(_x);
        square_ennemy->getPosition()->set<1>(_y);
        square_ennemy->getCanShoot()->set<0>(true);
        if (*square_ennemy->getWeapon()->get<0>() == WeaponType::BIG_SHOT) {
            square_ennemy->getCanShoot()->set<1>(3.0F);
        } else {
            square_ennemy->getCanShoot()->set<1>(1.5F);
        }
        square_ennemy->getSprite()->set<0>(_spriteId);

        auto _netId = networkManager.getnewNetID();

        square_ennemy->getNetworkID()->set<0>(_netId);

        server.send_tcp(
            RTypePacketType::NEW_ENNEMY,
            net::Packet::serializeStruct(NewEnnemy {_x, _y, _netId, rand})
        );
    }

    ennemyCount = 5;
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
    C::ChunkPosPool::Types &cchunkpos, C::NetworkIDPool::Types &cnetworkid
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
    AStatusMonoSystem(false, C::ENT_ALIVE),
    entityManager(entityManager),
    networkManager(networkManager),
    server(server),
    _spriteId(spriteId)
{
}

void ShootSystem::_statusOperate(
    C::PositionPool::Types &cposition, C::TypePool::Types &ctype, C::CanShootPool::Types &canshoot,
    C::IsShootingPool::Types &cIsShooting, C::WeaponPool::Types &cweapon
)
{
    auto [type] = ctype;
    auto [canShoot, base_delay, delay] = canshoot;
    auto [isShooting] = cIsShooting;
    if (canShoot == 0) {
        return;
    }
    if (delay > 0) {
        delay -= deltaTime;
        return;
    }
    auto [weapon] = cweapon;
    if (((isShooting != 0) && type == GameEntityType::PLAYER) || type == GameEntityType::ENEMY) {
        auto [x, y] = cposition;
        Vector2 playerPosition = {x, y};
        if (type == GameEntityType::ENEMY) {
            auto [ennemyX, ennemyY] = cposition;

            float closestDistance = std::numeric_limits<float>::max();
            float closestPlayerX = 0;
            float closestPlayerY = 0;

            if (playersPos.empty()) {
                return;
            }
            for (const auto &playerPos : playersPos) {
                float playerX = playerPos.x;
                float playerY = playerPos.y;
                auto distance = (float
                ) std::sqrt(std::pow(playerX - ennemyX, 2) + std::pow(playerY - ennemyY, 2));

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
        auto bullets = entityManager.createEntities("GameEntity", 1, ECS::C::ENT_ALIVE);

        for (const auto &entity : bullets) {
            auto ref = entityManager.getEntity(entity);

            auto *square_bullet = dynamic_cast<ECS::E::GameAnimatedEntityRef *>(ref.get());
            if (square_bullet == nullptr) {
                std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
                return;
            }
            if (type == GameEntityType::PLAYER) {
                square_bullet->getVelocity()->set<0>(500.0F);
                square_bullet->getPosition()->set<0>(x + 80 + 35);
                square_bullet->getRotation()->set<0>(90);
                square_bullet->getType()->set<0>(GameEntityType::BULLET);
            } else {
                square_bullet->getVelocity()->set<0>(-500.0F);
                square_bullet->getPosition()->set<0>(x - 35);
                square_bullet->getRotation()->set<0>(-90);
                square_bullet->getType()->set<0>(GameEntityType::BULLET_ENNEMY);
            }
            square_bullet->getVelocity()->set<1>(0.0F);
            square_bullet->getVelocity()->set<2>(300.0F);
            square_bullet->getColor()->set<0>(255);
            square_bullet->getColor()->set<1>(255);
            square_bullet->getColor()->set<2>(0);
            square_bullet->getColor()->set<3>(255);
            square_bullet->getPosition()->set<1>(y + 25);
            square_bullet->getCanShoot()->set<0>(false);
            if (weapon == WeaponType::BULLET) {
                square_bullet->getSprite()->set<0>(_spriteId);
                square_bullet->getSprite()->set<2>(30.0F);
                square_bullet->getSprite()->set<3>(30.0F);
                square_bullet->getSprite()->set<4>(4.0F);
                square_bullet->getHealth()->set<0>(1);
                square_bullet->getSize()->set<0>(30);
                square_bullet->getSize()->set<1>(30);
            }
            if (weapon == WeaponType::BIG_SHOT) {
                square_bullet->getSprite()->set<0>(_spriteId);
                square_bullet->getSprite()->set<2>(70.0F);
                square_bullet->getSprite()->set<3>(70.0F);
                square_bullet->getSprite()->set<4>(10.0F);
                square_bullet->getHealth()->set<0>(5);
                square_bullet->getSize()->set<0>(70);
                square_bullet->getSize()->set<1>(70);
            }
            square_bullet->getSprite()->set<1>(true);
            square_bullet->getSprite()->set<5>(0);
            square_bullet->getTimer()->set<0>(0.0F);

            auto _netId = networkManager.getnewNetID();
            square_bullet->getNetworkID()->set<0>(_netId);
            if (type == GameEntityType::PLAYER) {
                if (weapon == WeaponType::BULLET) {
                    server.send_tcp(
                        RTypePacketType::BULLET_SHOT,
                        net::Packet::serializeStruct(
                            BulletShot {x + 80 + 35, y + 25, true, _netId, false}
                        )
                    );
                }
                if (weapon == WeaponType::BIG_SHOT) {
                    server.send_tcp(
                        RTypePacketType::BULLET_SHOT,
                        net::Packet::serializeStruct(
                            BulletShot {x + 80 + 35, y + 25, true, _netId, true}
                        )
                    );
                }
            } else {
                if (weapon == WeaponType::BULLET) {
                    server.send_tcp(
                        RTypePacketType::BULLET_SHOT,
                        net::Packet::serializeStruct(
                            BulletShot {x - 35, y + 25, false, _netId, false}
                        )
                    );
                }
                if (weapon == WeaponType::BIG_SHOT) {
                    server.send_tcp(
                        RTypePacketType::BULLET_SHOT,
                        net::Packet::serializeStruct(
                            BulletShot {x - 35, y + 25, false, _netId, true}
                        )
                    );
                }
            }
        }
    }
}

CountEnnemyAliveSystem::CountEnnemyAliveSystem(size_t &ennemyCount):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    ennemyCount(ennemyCount)
{
}

void CountEnnemyAliveSystem::_statusOperate(C::TypePool::Types &ctype)
{
    auto [type] = ctype;
    if (type == GameEntityType::ENEMY) {
        ennemyCount++;
    }
}

SendAllDataToNewClients::SendAllDataToNewClients():
    AStatusMonoSystem(false, C::ENT_ALIVE)
{
}

void SendAllDataToNewClients::_statusOperate(
    C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity, C::TypePool::Types &ctype,
    C::HealthPool::Types &chealth, C::NetworkIDPool::Types &cnetworkid
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
    C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity, C::TypePool::Types &ctype,
    C::NetworkIDPool::Types &cnetworkid, C::IsShootingPool::Types &cIsShooting
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