/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "DecorEntities.hpp"
#include "GameEntities.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include <cmath>
#include <limits>
#include "lib_ecs/Systems/ADualSystem.hpp"

namespace ECS {
namespace S {
// SYSTEM

MoveBackgroundSystem::MoveBackgroundSystem():
    AMonoSystem(false)
{
}

void MoveBackgroundSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::TypePool::Types &ctype
)
{
    auto [status] = cstatus;
    auto [type] = ctype;

    if (status == C::EntityStatusEnum::ENT_ALIVE && type != GameEntityType::BACKGROUND &&
        type != GameEntityType::WALL) {
        auto [x, y] = cposition;
        if (x < cameraX - 1000) {
            status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
        if (type == GameEntityType::BULLET && x > cameraX + 1000) {
            status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
    }

    if (status != C::EntityStatusEnum::ENT_ALIVE || type != GameEntityType::BACKGROUND) {
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

    if (type != GameEntityType::ENEMY) {
        return;
    }
    if (playersPos.empty()) {
        return;
    }

    auto [ennemyX, ennemyY] = cposition;
    auto [vX, vY] = cvelocity;
    vX = 0;
    vY = 0;

    float closestDistance = std::numeric_limits<float>::max();
    float closestPlayerX = 0;
    float closestPlayerY = 0;

    for (const auto &playerPos : playersPos) {
        float playerX = playerPos.x;
        float playerY = playerPos.y;
        auto distance = (float) std::sqrt(std::pow(playerX - ennemyX, 2) + std::pow(playerY - ennemyY, 2));

        if (distance < closestDistance) {
            closestDistance = distance;
            closestPlayerX = playerX;
            closestPlayerY = playerY;
        }
    }

    if ((ennemyX - closestPlayerX) > 1000) {
        return;
    }
    if (closestPlayerY < ennemyY) {
        vY = -150;
    }
    if (closestPlayerY > ennemyY) {
        vY = 150;
    }
    if (closestPlayerY < ennemyY && closestPlayerY + 50 > ennemyY) {
        vY = 0;
    }
}

ColliderSystem::ColliderSystem():
    ASelfDualSystem(false)
{
}

void ColliderSystem::_innerOperate(
    C::EntityStatusPool::Types &cStatusA, C::PositionPool::Types &cpositionA,
    C::SizePool::Types &csizeA, C::TypePool::Types &ctypeA, C::HealthPool::Types &chealthA,
    C::EntityStatusPool::Types &cStatusB, C::PositionPool::Types &cpositionB,
    C::SizePool::Types &csizeB, C::TypePool::Types &ctypeB, C::HealthPool::Types &chealthB
)
{
    auto [statusA] = cStatusA;
    auto [statusB] = cStatusB;

    if (statusA != C::EntityStatusEnum::ENT_ALIVE || statusB != C::EntityStatusEnum::ENT_ALIVE) {
        return;
    }
    auto [xA, yA] = cpositionA;
    auto [sizeXA, sizeYA] = csizeA;
    auto [typeA] = ctypeA;

    auto [xB, yB] = cpositionB;
    auto [sizeXB, sizeYB] = csizeB;
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
    if ((typeA == GameEntityType::BULLET || typeA == GameEntityType::PLAYER) &&
        (typeB == GameEntityType::BULLET || typeB == GameEntityType::PLAYER)) {
        return;
    }
    if ((typeA == GameEntityType::BULLET_ENNEMY || typeA == GameEntityType::ENEMY) &&
        (typeB == GameEntityType::BULLET_ENNEMY || typeB == GameEntityType::ENEMY)) {
        return;
    }
    if ((typeA == GameEntityType::WALL && typeB == GameEntityType::WALL)) {
        return;
    }

    // Check if entities are colliding
    if (xA < xB + sizeXB && xA + sizeXA > xB && yA < yB + sizeYB && yA + sizeYA > yB) {
        collide = true;
    }

    if (collide) {
        healthA -= 1;
        healthB -= 1;
    }

    // Set flags to destroy entities
    if (healthA <= 0 && typeA != GameEntityType::WALL) {
        statusA = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
    }
    if (healthB <= 0 && typeB != GameEntityType::WALL) {
        statusB = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
    }
}

GetPlayerPositionSystem::GetPlayerPositionSystem():
    AStatusMonoSystem(false, C::ENT_ALIVE)
{
}

void GetPlayerPositionSystem::_statusOperate(
    C::EntityStatusPool::Types &centitystatus, C::PositionPool::Types &cposition,
    C::TypePool::Types &ctype
)
{
    auto [entityStatus] = centitystatus;
    auto [type] = ctype;
    if ((type != GameEntityType::LPLAYER && type != GameEntityType::PLAYER) ||
        entityStatus != C::ENT_ALIVE) {
        return;
    }
    auto [x, y] = cposition;
    playersPos.push_back({x, y});
}

} // namespace S
} // namespace ECS