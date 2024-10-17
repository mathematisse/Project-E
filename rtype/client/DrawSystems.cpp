/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "DrawSystems.hpp"
#include "DecorSquare.hpp"
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
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::ColorPool::Types &ccolor, C::SizePool::Types &csize
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
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::SizePool::Types &csize, C::TypePool::Types &ctype, C::SpritePool::Types &csprite
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

AnimationSystem::AnimationSystem(AssetsLoader &assetsLoader):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    assetsLoader(assetsLoader)
{
}

void AnimationSystem::_statusOperate(C::SpritePool::Types &csprite, C::TimerPool::Types &ctimer)
{
    float deltaTime = GetFrameTime();
    auto [id, animated, x, y, nbr_frame, sprite_pos, animation_time] = csprite;
    auto [clock, end_clock] = ctimer;
    clock += deltaTime * 100;
    auto texture = assetsLoader.get_asset_from_id(id);
    if (clock >= end_clock) {
        clock = 0;
        if (sprite_pos < (float) texture.width) {
            sprite_pos += (float) (texture.width) / nbr_frame;
        } else {
            sprite_pos = 0;
        }
    }
}

} // namespace S
} // namespace ECS