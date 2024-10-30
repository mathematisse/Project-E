/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "Components.hpp"
#include <cmath>
#include <limits>
#include "lib_ecs/Systems/ADualSystem.hpp"
#include "Components.hpp"

namespace ECS::S {
// SYSTEM

MoveBackgroundSystem::MoveBackgroundSystem():
    AMonoSystem(false)
{
}

void MoveBackgroundSystem::_innerOperate(
    C::EntityStatus::Pool::Types &cstatus, C::Position::Pool::Types &cposition,
    C::Type::Pool::Types &ctype
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
    if (cameraX > x + 6000) {
        x += 9000;
    }
}

MoveEnnemySystem::MoveEnnemySystem():
    AMonoSystem(false)
{
}

void MoveEnnemySystem::_innerOperate(
    C::Position::Pool::Types &cposition, C::Velocity::Pool::Types &cvelocity,
    C::Type::Pool::Types &ctype
)
{
    auto [type] = ctype;

    if (type != GameEntityType::ENEMY) {
        return;
    }
    if (playersPos.empty()) {
        return;
    }

    auto [enemyX, enemyY] = cposition;
    auto [vX, vY] = cvelocity;
    vX = 0;
    vY = 0;

    float closestDistance = std::numeric_limits<float>::max();
    float closestPlayerX = 0;
    float closestPlayerY = 0;

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

    if ((enemyX - closestPlayerX) > 1000) {
        return;
    }
    if (closestPlayerY < enemyY) {
        vY = -150;
    }
    if (closestPlayerY > enemyY) {
        vY = 150;
    }
    if (closestPlayerY < enemyY && closestPlayerY + 50 > enemyY) {
        vY = 0;
    }
}

ColliderSystem::ColliderSystem():
    ASelfDualSystem(false)
{
}

void ColliderSystem::_innerOperate(
    C::EntityStatus::Pool::Types &cStatusA, C::Position::Pool::Types &cpositionA,
    C::Size::Pool::Types &csizeA, C::Type::Pool::Types &ctypeA, C::Health::Pool::Types &chealthA,
    C::EntityStatus::Pool::Types &cStatusB, C::Position::Pool::Types &cpositionB,
    C::Size::Pool::Types &csizeB, C::Type::Pool::Types &ctypeB, C::Health::Pool::Types &chealthB
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

    // Check if entities are colliding with walls
    if (yA < 100 || yA + sizeYA > 980) {
        healthA = 0;
    }
    if (yB < 100 || yB + sizeYB > 980) {
        healthB = 0;
    }

    // only player collides with powerups
    if ((typeA == GameEntityType::POWERUP && typeB != GameEntityType::PLAYER) ||
        (typeB == GameEntityType::POWERUP && typeA != GameEntityType::PLAYER)) {
        return;
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
    if (xA >= xB + sizeXB || xA + sizeXA <= xB || yA >= yB + sizeYB || yA + sizeYA <= yB) {
        return;
    }
    LOG_DEBUG(LOG_YELLOW "COLLISION" LOG_COLOR_RESET);

    if (typeA == GameEntityType::POWERUP) {
        healthA = 0;
        healthB += 50;
    } else if (typeB == GameEntityType::POWERUP) {
        healthB = 0;
        healthA += 50;
    } else {
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
    AMonoSystem(false)
{
}

void GetPlayerPositionSystem::_innerOperate(
    C::EntityStatus::Pool::Types &centitystatus, C::Position::Pool::Types &cposition,
    C::Type::Pool::Types &ctype
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

void ChangePlayerWeaponSystem::_innerOperate(
    C::Type::Pool::Types &ctype, C::Health::Pool::Types &cHealth, C::Weapon::Pool::Types &cweapon
)
{
    auto [type] = ctype;
    if (type != GameEntityType::PLAYER) {
        return;
    }
    auto [health] = cHealth;
    if (health < 50) {
        return;
    }
    auto [weapon] = cweapon;
    if (weapon == WeaponType::BULLET) {
        weapon = WeaponType::BIG_SHOT;
    } else {
        weapon = WeaponType::BULLET;
    }
    health -= 50;
}

} // namespace ECS::S
