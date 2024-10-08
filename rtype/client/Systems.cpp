/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "Square.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include <iomanip>
#include <iostream>
#include "AssetsPath.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include <raylib.h>

#define RED_CLI "\033[31m"
#define GREEN_CLI "\033[32m"
#define YELLOW_CLI "\033[33m"
#define RESET "\033[0m"

namespace ECS {
namespace S {
// SYSTEM

DrawSystem::DrawSystem():
    AMonoSystem(false)
{
}

void DrawSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition, C::ColorPool::Types &ccolor,
    C::SizePool::Types &csize
)
{
    auto [status] = cstatus;
    if (status != C::EntityStatusEnum::ENT_ALIVE || !IsKeyDown(KEY_H)) {
        return;
    }
    auto [x, y] = cposition;
    auto [r, g, b, a] = ccolor;
    auto [sizeX, sizeY, rotation] = csize;
    DrawRectangle((int) x, (int) y, sizeX, sizeY, {r, g, b, a});
    EndMode2D();
}

DrawSpriteSystem::DrawSpriteSystem(AssetsLoader &assetsLoader, Camera2D &camera):
    AMonoSystem(false),
    assetsLoader(assetsLoader),
    camera(camera)
{
}

void DrawSpriteSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition, C::SizePool::Types &csize,
    C::TypePool::Types &ctype, C::SpritePool::Types &csprite
)
{
    auto [status] = cstatus;
    if (status != C::EntityStatusEnum::ENT_ALIVE || IsKeyDown(KEY_H)) {
        return;
    }
    auto [sprite_id] = csprite;
    if (sprite_id == 0) {
        return;
    }
    auto [x, y] = cposition;
    auto [sizeX, sizeY, rotation] = csize;
    auto [type] = ctype;
    auto texture = assetsLoader.get_asset_from_id(sprite_id);
    float scale = texture.width / sizeX;
    float adjustedX = x;
    float adjustedY = y;
    if (rotation == 90) {
        adjustedX += sizeX;
    }
    if (rotation == -90) {
        adjustedY += sizeY;
    }
    if (type != SquareType::BACKGROUND &&
        (adjustedX < camera.target.x - 1200 || adjustedX > camera.target.x + 1200)) {
        return;
    }
    BeginMode2D(camera);
    DrawTextureEx(texture, {adjustedX, adjustedY}, rotation, 1 / scale, WHITE);
    if (type == SquareType::BACKGROUND) {
        DrawTextureEx(texture, {adjustedX - 3000, adjustedY}, rotation, 1 / scale, WHITE);
        DrawTextureEx(texture, {adjustedX + 3000, adjustedY}, rotation, 1 / scale, WHITE);
    }
}

ApplyVelocitySystem::ApplyVelocitySystem():
    AMonoSystem(false)
{
}

void ApplyVelocitySystem::_innerOperate(C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity)
{
    auto [x, y] = cposition;
    auto [vX, vY, speed] = cvelocity;
    x += vX * deltaTime * speed;
    y += vY * deltaTime * speed;
}

MovePlayerSystem::MovePlayerSystem():
    AMonoSystem(false)
{
}

void MovePlayerSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::VelocityPool::Types &cvelocity, C::TypePool::Types &ctype
)
{
    auto [status] = cstatus;
    auto [type] = ctype;
    if (status != C::EntityStatusEnum::ENT_ALIVE || type != SquareType::PLAYER) {
        return;
    }
    auto [vX, vY, _] = cvelocity;

    vX = 0;
    vY = 0;
    if (IsKeyDown(KEY_UP)) {
        vY -= 1;
    }
    if (IsKeyDown(KEY_DOWN)) {
        vY += 1;
    }
    if (IsKeyDown(KEY_LEFT)) {
        vX -= 1;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        vX += 1;
    }
}

SpawnEnnemySystem::SpawnEnnemySystem(
    EntityManager &entityManager, AssetsLoader &assetsLoader, Camera2D &camera, size_t maxEnnemyCount
):
    AMonoSystem(false),
    entityManager(entityManager),
    assetsLoader(assetsLoader),
    camera(camera),
    _maxEnnemyCount(maxEnnemyCount)
{
}

void SpawnEnnemySystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition, C::TypePool::Types &ctype
)
{
    auto [status] = cstatus;
    auto [type] = ctype;

    if (status == C::EntityStatusEnum::ENT_ALIVE && type != SquareType::BACKGROUND &&
        type != SquareType::WALL) {
        auto [x, y] = cposition;
        if (type == SquareType::ENEMY && x < camera.target.x - 1000) {
            _ennemyCount--;
            status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        } else if (x < camera.target.x - 1000) {
            status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
    }

    if (status != C::EntityStatusEnum::ENT_ALIVE || type != SquareType::PLAYER) {
        return;
    }
    auto need_to_spawn = _maxEnnemyCount - _ennemyCount;
    if (need_to_spawn <= 0) {
        return;
    }
    auto [x, y] = cposition;
    auto ennemies = entityManager.createEntities("Square", need_to_spawn, ECS::C::ENT_ALIVE);

    for (const auto &entity : ennemies) {
        auto ref = entityManager.getEntity(entity);

        auto square_ennemy = dynamic_cast<ECS::E::SquareRef *>(ref.get());
        if (!square_ennemy) {
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

        square_ennemy->getPosition()->set<0>(x + 500 + rand() % (int) (x + 1000));
        square_ennemy->getPosition()->set<1>(100 + rand() % 800);
        square_ennemy->getCanShoot()->set<0>(true);
        square_ennemy->getCanShoot()->set<1>(1.5F);
        square_ennemy->getSprite()->set<0>(assetsLoader.get_asset(E1FC).id);
    }
    _ennemyCount += need_to_spawn;
}

ShootSystem::ShootSystem(EntityManager &entityManager, AssetsLoader &assetsLoader):
    AMonoSystem(false),
    entityManager(entityManager),
    assetsLoader(assetsLoader)
{
}

void ShootSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition, C::TypePool::Types &ctype,
    C::CanShootPool::Types &canshoot
)
{
    auto [status] = cstatus;
    auto [type] = ctype;
    auto [canShoot, base_delay, delay] = canshoot;
    if (status != C::EntityStatusEnum::ENT_ALIVE || !canShoot) {
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

            auto square_bullet = dynamic_cast<ECS::E::SquareRef *>(ref.get());
            if (!square_bullet) {
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
            square_bullet->getSprite()->set<0>(assetsLoader.get_asset(CUT_BULLET_PATH).id);
        }
    }
}

MoveBackgroundSystem::MoveBackgroundSystem(Camera2D &camera):
    AMonoSystem(false),
    camera(camera)
{
}

void MoveBackgroundSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition, C::TypePool::Types &ctype
)
{
    auto [status] = cstatus;
    auto [type] = ctype;
    if (status != C::EntityStatusEnum::ENT_ALIVE || type != SquareType::BACKGROUND) {
        return;
    }
    auto [x, y] = cposition;
    if (camera.target.x > x + 3000) {
        x += 3000;
    }
}

MoveEnnemySystem::MoveEnnemySystem():
    AMonoSystem(false)
{
}

void MoveEnnemySystem::_innerOperate(
    C::EntityStatusPool::Types &cStatus, C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity,
    C::TypePool::Types &ctype
)
{
    auto [status] = cStatus;
    if (status != C::EntityStatusEnum::ENT_ALIVE) {
        return;
    }
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

    if ((ennemyX - playerX) > 1000)
        return;
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
    C::EntityStatusPool::Types &cStatusA, C::PositionPool::Types &cpositionA,
    C::VelocityPool::Types & /*unused*/, C::SizePool::Types &csizeA, C::TypePool::Types &ctypeA,
    C::EntityStatusPool::Types &cStatusB, C::PositionPool::Types &cpositionB,
    C::VelocityPool::Types & /*unused*/, C::SizePool::Types &csizeB, C::TypePool::Types &ctypeB
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
    bool collide = false;

    if (xA + sizeXA > xB && xA < xB + sizeXB && yA + sizeYA > yB && yA < yB + sizeYB) {
        collide = true;
    }

    if (collide) {
        if (typeA == SquareType::PLAYER && typeB == SquareType::ENEMY) {
            statusA = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
            statusB = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
        if (typeA == SquareType::ENEMY && typeB == SquareType::PLAYER) {
            statusA = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
            statusB = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
        if (typeA == SquareType::BULLET && typeB == SquareType::ENEMY) {
            statusA = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
            statusB = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
        if (typeA == SquareType::BULLET_ENNEMY && typeB == SquareType::PLAYER) {
            statusA = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
            statusB = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
        if (typeA == SquareType::BULLET || typeA == SquareType::BULLET_ENNEMY) {
            statusA = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
        if (typeB == SquareType::BULLET || typeB == SquareType::BULLET_ENNEMY) {
            statusB = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
    }
}

} // namespace S
} // namespace ECS