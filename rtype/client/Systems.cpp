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

DrawSystem::DrawSystem(Camera2D &camera):
    AMonoSystem(false),
    camera(camera)
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
    BeginMode2D(camera);
    DrawRectangle((int) x, (int) y, sizeX, sizeY, {r, g, b, a});
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
    auto [id, flag, sprite_x, sprite_y, nbr_frame, start_position, animation_time] = csprite;
    if (id == 0) {
        return;
    }
    auto [x, y] = cposition;
    auto [sizeX, sizeY, rotation] = csize;
    auto [type] = ctype;
    auto texture = assetsLoader.get_asset_from_id(id);
    float scale = texture.width / sizeX;
    float adjustedX = x;
    float adjustedY = y;
    if (flag == 0) {
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
    } else {
        if (rotation == 90) {
            DrawTexturePro(
                texture, {start_position, 0, texture.width / nbr_frame, (float) texture.height},
                {x, y, sprite_x, sprite_y}, Vector2 {0, 0}, rotation, WHITE
            );
        } else if (rotation == -90 || rotation == 270) {
            DrawTexturePro(
                texture, {start_position, 0, texture.width / nbr_frame, (float) texture.height},
                {x, y + sprite_y, sprite_x, sprite_y}, Vector2 {0, 0}, rotation, WHITE
            );
        }
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

CountEnnemyAliveSystem::CountEnnemyAliveSystem(size_t &ennemyCount):
    AMonoSystem(false),
    ennemyCount(ennemyCount)
{
}

void CountEnnemyAliveSystem::_innerOperate(C::EntityStatusPool::Types &cstatus, C::TypePool::Types &ctype)
{
    auto [status] = cstatus;
    auto [type] = ctype;
    if (status == C::EntityStatusEnum::ENT_ALIVE && type == SquareType::ENEMY) {
        ennemyCount++;
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

    if (status != C::EntityStatusEnum::ENT_ALIVE || type != SquareType::PLAYER) {
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
        square_ennemy->getHealth()->set<0>(2);
    }
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
            square_bullet->getSprite()->set<0>(assetsLoader.get_asset(BASE_BULLET_PATH).id);
            square_bullet->getSprite()->set<1>(true);
            square_bullet->getSprite()->set<2>(30.0F);
            square_bullet->getSprite()->set<3>(30.0F);
            square_bullet->getSprite()->set<4>(4.0F);
            square_bullet->getSprite()->set<5>(0);
            square_bullet->getHealth()->set<0>(1);
            square_bullet->getTimer()->set<0>(0.0F);
            square_bullet->getTimer()->set<1>(8.0F);
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

    if (status == C::EntityStatusEnum::ENT_ALIVE && type != SquareType::BACKGROUND &&
        type != SquareType::WALL) {
        auto [x, y] = cposition;
        if (x < camera.target.x - 1000) {
            status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
        if (type == SquareType::BULLET && x > camera.target.x + 1000) {
            status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        }
    }

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

    if (xA + sizeXA > xB && xA < xB + sizeXB && yA + sizeYA > yB && yA < yB + sizeYB) {
        collide = true;
    }

    if (yA < 100 || yA + sizeYA > 980) {
        healthA = 0;
    }

    if (yB < 100 || yB + sizeYB > 980) {
        healthB = 0;
    }

    if (collide) {
        if (typeA == SquareType::PLAYER && typeB == SquareType::ENEMY) {
            healthA -= 1;
            healthB -= 1;
        }
        if (typeA == SquareType::ENEMY && typeB == SquareType::PLAYER) {
            healthA -= 1;
            healthB -= 1;
        }
        if (typeA == SquareType::BULLET && typeB == SquareType::ENEMY) {
            healthA -= 1;
            healthB -= 1;
        }
        if (typeA == SquareType::BULLET_ENNEMY && typeB == SquareType::PLAYER) {
            healthA -= 1;
            healthB -= 1;
        }
        if ((typeA == SquareType::BULLET || typeA == SquareType::BULLET_ENNEMY) &&
            typeB == SquareType::WALL) {
            healthA -= 1;
        }
        if ((typeB == SquareType::BULLET || typeB == SquareType::BULLET_ENNEMY) &&
            typeA == SquareType::WALL) {
            healthB -= 1;
        }

        if ((typeA == SquareType::BULLET && typeB == SquareType::BULLET_ENNEMY) ||
            ((typeB == SquareType::BULLET && typeA == SquareType::BULLET_ENNEMY))) {
            healthA -= 1;
            healthB -= 1;
        }
    }
    if (healthA <= 0) {
        statusA = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
    }
    if (healthB <= 0) {
        statusB = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
    }
}

ShowInfoSystem::ShowInfoSystem(Camera2D &camera):
    AMonoSystem(false),
    camera(camera)
{
}

void ShowInfoSystem::_innerOperate(C::TypePool::Types &ctype, C::HealthPool::Types &chealth)
{
    auto [type] = ctype;

    if (type == SquareType::PLAYER && !one_time) {
        one_time = true;
        auto [health] = chealth;
        Vector2 top_left = {camera.target.x - 1920 / 2, camera.target.y - 1080 / 2};
        DrawText("Player health: ", top_left.x + 10, top_left.y + 10, 20, RED);
        DrawText(std::to_string(health).c_str(), top_left.x + 200, top_left.y + 10, 20, RED);
    }
}

ClockSystem::ClockSystem(AssetsLoader &assetsLoader):
    AMonoSystem(false),
    assetsLoader(assetsLoader)
{
}

void ClockSystem::_innerOperate(C::SpritePool::Types &csprite, C::TimerPool::Types &ctimer)
{
    float deltaTime = GetFrameTime();
    auto [id, animated, x, y, nbr_frame, sprite_pos, animation_time] = csprite;
    auto [clock, end_clock] = ctimer;
    clock += deltaTime * 100;
    std::cout << "Clock: " << clock << std::endl;
    auto texture = assetsLoader.get_asset_from_id(id);
    if (clock >= end_clock) {

        std::cout << "postion " << sprite_pos << std::endl;
        clock = 0;
        if (sprite_pos < (float) texture.width) {
            sprite_pos += (float) (texture.width) / nbr_frame;
            std::cout << "new postion " << sprite_pos << std::endl;
        } else {
            sprite_pos = 0;
        }
    }
}

} // namespace S
} // namespace ECS