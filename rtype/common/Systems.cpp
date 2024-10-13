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
#include <cmath>
#include <limits>
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

void ApplyVelocitySystem::_statusOperate(
    C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity
)
{
    auto [x, y] = cposition;
    auto [vX, vY, speed] = cvelocity;
    x += vX * deltaTime * speed;
    y += vY * deltaTime * speed;
}

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
    if (playersPos.empty()) {
        return;
    }

    auto [ennemyX, ennemyY] = cposition;
    auto [vX, vY, speed] = cvelocity;
    vX = 0;
    vY = 0;

    float closestDistance = std::numeric_limits<float>::max();
    float closestPlayerX = 0;
    float closestPlayerY = 0;

    for (const auto &playerPos : playersPos) {
        float playerX = playerPos.x;
        float playerY = playerPos.y;
        float distance = std::sqrt(std::pow(playerX - ennemyX, 2) + std::pow(playerY - ennemyY, 2));

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
        vY = -1;
    }
    if (closestPlayerY > ennemyY) {
        vY = 1;
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
    if (xA < xB + sizeXB && xA + sizeXA > xB && yA < yB + sizeYB && yA + sizeYA > yB) {
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
    if ((type != SquareType::LPLAYER && type != SquareType::PLAYER) ||
        entityStatus != C::ENT_ALIVE) {
        return;
    }
    auto [x, y] = cposition;
    playersPos.push_back({x, y});
}

} // namespace S
} // namespace ECS