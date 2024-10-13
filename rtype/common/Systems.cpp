/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "DecorSquare.hpp"
#include "Square.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include <iostream>
#include "lib_ecs/Systems/ADualSystem.hpp"

#define RED_CLI "\033[31m"
#define GREEN_CLI "\033[32m"
#define YELLOW_CLI "\033[33m"
#define RESET "\033[0m"

namespace ECS {
namespace S {
// SYSTEM

ApplyVelocitySystem::ApplyVelocitySystem():
    AStatusMonoSystem(true, C::ENT_ALIVE)
{
}

void ApplyVelocitySystem::_statusOperate(C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity)
{
    auto [x, y] = cposition;
    auto [vX, vY, speed] = cvelocity;
    x += vX * deltaTime * speed;
    y += vY * deltaTime * speed;
}

CountEnnemyAliveSystem::CountEnnemyAliveSystem(size_t &ennemyCount):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    ennemyCount(ennemyCount)
{
}

void CountEnnemyAliveSystem::_statusOperate(C::TypePool::Types &ctype)
{
    auto [type] = ctype;
    if (type == SquareType::ENEMY) {
        ennemyCount++;
    }
}

SpawnEnnemySystem::SpawnEnnemySystem(
    EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId, size_t maxEnnemyCount
):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    entityManager(entityManager),
    networkManager(networkManager),
    _spriteId(spriteId),
    _maxEnnemyCount(maxEnnemyCount)
{
}

void SpawnEnnemySystem::_statusOperate(C::PositionPool::Types &cposition, C::TypePool::Types &ctype)
{
    auto [type] = ctype;

    if (type != SquareType::PLAYER) {
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
    auto ennemies = entityManager.createEntities("Square", need_to_spawn, ECS::C::ENT_ALIVE);

    for (const auto &entity : ennemies) {
        auto ref = entityManager.getEntity(entity);

        auto *square_ennemy = dynamic_cast<ECS::E::SquareRef *>(ref.get());
        if (square_ennemy == nullptr) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return;
        }
        square_ennemy->getVelocity()->set<0>(0.0F);
        square_ennemy->getVelocity()->set<1>(0.0F);
        square_ennemy->getVelocity()->set<2>(150.0F);
        square_ennemy->getType()->set<0>(SquareType::ENEMY);
        square_ennemy->getColor()->set<0>(255);
        square_ennemy->getColor()->set<1>(0);
        square_ennemy->getColor()->set<2>(0);
        square_ennemy->getColor()->set<3>(255);
        square_ennemy->getSize()->set<0>(80);
        square_ennemy->getSize()->set<1>(80);
        square_ennemy->getSize()->set<2>(90);

        float _x = x + 500 + rand() % (int) (x + 1000);
        float _y = 100 + rand() % 800;
        square_ennemy->getPosition()->set<0>(_x);
        square_ennemy->getPosition()->set<1>(_y);
        square_ennemy->getCanShoot()->set<0>(true);
        square_ennemy->getCanShoot()->set<1>(1.5F);
        square_ennemy->getSprite()->set<0>(_spriteId);
        square_ennemy->getHealth()->set<0>(2);
        auto _netId = networkManager.getnewNetID();

        square_ennemy->getNetworkID()->set<0>(_netId);
    }
}

DestroyEntitiesSystem::DestroyEntitiesSystem(EntityManager &entityManager):
    AStatusMonoSystem(false, C::ENT_NEEDS_DESTROY),
    entityManager(entityManager)
{
}

void DestroyEntitiesSystem::_statusOperate(C::ChunkPosPool::Types &cchunkpos)
{
    entityManager.destroyEntity(cchunkpos);
}

ShootSystem::ShootSystem(EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId = 0):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    entityManager(entityManager),
    networkManager(networkManager),
    playerPosition({0, 0}),
    _spriteId(spriteId)
{
}

void ShootSystem::_statusOperate(
    C::PositionPool::Types &cposition, C::TypePool::Types &ctype, C::CanShootPool::Types &canshoot
)
{
    auto [type] = ctype;
    auto [canShoot, base_delay, delay] = canshoot;
    if (canShoot == 0) {
        return;
    }
    if (delay > 0) {
        delay -= deltaTime;
        return;
    }
    if ((IsKeyDown(KEY_SPACE) && type == SquareType::PLAYER) || type == SquareType::ENEMY) {
        auto [x, y] = cposition;
        if (type == SquareType::ENEMY &&
            ((x - playerPosition.x) > 1000 || x <= playerPosition.x || y < playerPosition.y ||
             y > playerPosition.y + 50)) {
            return;
        }
        delay = base_delay;
        auto bullets = entityManager.createEntities("Square", 1, ECS::C::ENT_ALIVE);

        for (const auto &entity : bullets) {
            auto ref = entityManager.getEntity(entity);

            auto *square_bullet = dynamic_cast<ECS::E::SquareRef *>(ref.get());
            if (square_bullet == nullptr) {
                std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
                return;
            }
            if (type == SquareType::PLAYER) {
                square_bullet->getVelocity()->set<0>(1.0F);
                square_bullet->getPosition()->set<0>(x + 80 + 35);
                square_bullet->getSize()->set<2>(90);
                square_bullet->getType()->set<0>(SquareType::BULLET);
            } else {
                square_bullet->getVelocity()->set<0>(-1.0F);
                square_bullet->getPosition()->set<0>(x - 35);
                square_bullet->getSize()->set<2>(-90);
                square_bullet->getType()->set<0>(SquareType::BULLET_ENNEMY);
            }
            square_bullet->getVelocity()->set<1>(0.0F);
            square_bullet->getVelocity()->set<2>(500.0F);
            square_bullet->getColor()->set<0>(255);
            square_bullet->getColor()->set<1>(255);
            square_bullet->getColor()->set<2>(0);
            square_bullet->getColor()->set<3>(255);
            square_bullet->getPosition()->set<1>(y + 25);
            square_bullet->getCanShoot()->set<0>(false);
            square_bullet->getSize()->set<0>(30);
            square_bullet->getSize()->set<1>(30);
            square_bullet->getSprite()->set<0>(_spriteId);
            square_bullet->getSprite()->set<1>(true);
            square_bullet->getSprite()->set<2>(30.0F);
            square_bullet->getSprite()->set<3>(30.0F);
            square_bullet->getSprite()->set<4>(4.0F);
            square_bullet->getSprite()->set<5>(0);
            square_bullet->getHealth()->set<0>(1);
            square_bullet->getTimer()->set<0>(0.0F);
            square_bullet->getTimer()->set<1>(8.0F);

            auto _netId = networkManager.getnewNetID();
            square_bullet->getNetworkID()->set<0>(_netId);
        }
    }
}

MoveBackgroundSystem::MoveBackgroundSystem():
    AMonoSystem(false)
{
}

void MoveBackgroundSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition, C::TypePool::Types &ctype
)
{
    auto [status] = cstatus;
    auto [type] = ctype;

    if (status == C::EntityStatusEnum::ENT_ALIVE && type != SquareType::BACKGROUND &&
        type != SquareType::WALL) {
        auto [x, y] = cposition;
        if (x < cameraX - 1000) {
            status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
        if (type == SquareType::BULLET && x > cameraX + 1000) {
            status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
    }

    if (status != C::EntityStatusEnum::ENT_ALIVE || type != SquareType::BACKGROUND) {
        return;
    }
    auto [x, y] = cposition;
    if (cameraX > x + 3000) {
        x += 3000;
    }
}

MoveEnnemySystem::MoveEnnemySystem():
    AStatusMonoSystem(false, C::ENT_ALIVE)
{
}

void MoveEnnemySystem::_statusOperate(
    C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity, C::TypePool::Types &ctype
)
{
    auto [type] = ctype;

    if (type != SquareType::ENEMY) {
        return;
    }
    auto [ennemyX, ennemyY] = cposition;
    auto [vX, vY, speed] = cvelocity;
    float playerX = playerPosition.x;
    float playerY = playerPosition.y;
    vX = 0;
    vY = 0;

    if ((ennemyX - playerX) > 1000) {
        return;
    }
    if (playerY < ennemyY) {
        vY = -1;
    }
    if (playerY > ennemyY) {
        vY = 1;
    }
    if (playerY < ennemyY && playerY + 50 > ennemyY) {
        vY = 0;
    }
}

ColliderSystem::ColliderSystem():
    ASelfDualSystem(false)
{
}

void ColliderSystem::_innerOperate(
    C::EntityStatusPool::Types &cStatusA, C::PositionPool::Types &cpositionA, C::SizePool::Types &csizeA,
    C::TypePool::Types &ctypeA, C::HealthPool::Types &chealthA, C::EntityStatusPool::Types &cStatusB,
    C::PositionPool::Types &cpositionB, C::SizePool::Types &csizeB, C::TypePool::Types &ctypeB,
    C::HealthPool::Types &chealthB
)
{
    auto [statusA] = cStatusA;
    auto [statusB] = cStatusB;

    if (statusA != C::EntityStatusEnum::ENT_ALIVE || statusB != C::EntityStatusEnum::ENT_ALIVE) {
        return;
    }
    auto [xA, yA] = cpositionA;
    auto [sizeXA, sizeYA, _] = csizeA;
    auto [typeA] = ctypeA;

    auto [xB, yB] = cpositionB;
    auto [sizeXB, sizeYB, _2] = csizeB;
    auto [typeB] = ctypeB;
    auto [healthA] = chealthA;
    auto [healthB] = chealthB;

    bool collide = false;

    // Check if entities are colliding with walls
    if (yA < 100 || yA + sizeYA > 980) {
        healthA = 0;
    }
    if (yB < 100 || yB + sizeYB > 980) {
        healthB = 0;
    }

    // "Allies" can't collide
    if ((typeA == SquareType::BULLET || typeA == SquareType::PLAYER) &&
        (typeB == SquareType::BULLET || typeB == SquareType::PLAYER)) {
        return;
    }
    if ((typeA == SquareType::BULLET_ENNEMY || typeA == SquareType::ENEMY) &&
        (typeB == SquareType::BULLET_ENNEMY || typeB == SquareType::ENEMY)) {
        return;
    }
    if ((typeA == SquareType::WALL && typeB == SquareType::WALL)) {
        return;
    }

    // Check if entities are colliding
    if (xA + sizeXA > xB && xA < xB + sizeXB && yA + sizeYA > yB && yA < yB + sizeYB) {
        collide = true;
    }

    if (collide) {
        healthA -= 1;
        healthB -= 1;
    }

    // Set flags to destroy entities
    if (healthA <= 0 && typeA != SquareType::WALL) {
        statusA = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
    }
    if (healthB <= 0 && typeB != SquareType::WALL) {
        statusB = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
    }
}

} // namespace S
} // namespace ECS