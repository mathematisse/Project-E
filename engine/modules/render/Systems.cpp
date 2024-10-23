/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "spatial2d/Components.hpp"
#include <raylib.h>

namespace ECS {
namespace S {
// SYSTEM

DebugDrawSystem::DebugDrawSystem(Camera2D &camera):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    camera(camera)
{
}

void DebugDrawSystem::_statusOperate(
    C::PositionPool::Types &cposition, C::ColorPool::Types &ccolor,
    C::SizePool::Types &csize
)
{
    auto [x, y] = cposition;
    auto [r, g, b, a] = ccolor;
    auto [sizeX, sizeY] = csize;
    BeginMode2D(camera);
    DrawRectangle((int) x, (int) y, (int) sizeX, (int) sizeY, {r, g, b, a});
}

DrawSpriteSystem::DrawSpriteSystem(AssetsLoader &assetsLoader, Camera2D &camera):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    assetsLoader(assetsLoader),
    camera(camera)
{
}

void DrawSpriteSystem::_statusOperate(
    C::PositionPool::Types &cposition, C::SizePool::Types &csize, C::RotationPool::Types &crotation, C::SpritePool::Types &csprite
)
{
    auto [id] = csprite;
    if (id == 0) {
        return;
    }
    auto [x, y] = cposition;
    auto [sizeX, sizeY] = csize;
    auto [rotation] = crotation;
    auto texture = assetsLoader.get_asset_from_id(id);
    BeginMode2D(camera);
    DrawTextureEx(
        texture,
        {
            rotation == 90 ? x + sizeX : x,
            rotation == -90 ? y + sizeY : y
        },
        rotation,
        sizeX / (float) texture.width,
        WHITE
    );
}

DrawAnimatedSpriteSystem::DrawAnimatedSpriteSystem(AssetsLoader &assetsLoader, Camera2D &camera):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    assetsLoader(assetsLoader),
    camera(camera)
{
}

void DrawAnimatedSpriteSystem::_statusOperate(
    C::PositionPool::Types &cposition, C::SizePool::Types &csize, C::RotationPool::Types &crotation,
    C::AnimatedSpritePool::Types &csprite
)
{
    auto [id, nbr_frame, start_position, _] = csprite;
    if (id == 0) {
        return;
    }
    auto [x, y] = cposition;
    auto [sizeX, sizeY] = csize;
    auto [rotation] = crotation;
    auto texture = assetsLoader.get_asset_from_id(id);
    BeginMode2D(camera);
    DrawTexturePro(
        texture,
        {
            start_position,
            0,
            (float) texture.width / nbr_frame,
            (float) texture.height
        },
        {
            x,
            rotation == 90 ? y : y + sizeY,
            sizeX,
            sizeY
        },
        Vector2 {0, 0},
        rotation,
        WHITE
    );
}

SpriteAnimationSystem::SpriteAnimationSystem(AssetsLoader &assetsLoader):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    assetsLoader(assetsLoader)
{
}

void SpriteAnimationSystem::_statusOperate(C::AnimatedSpritePool::Types &csprite, C::TimerPool::Types &ctimer)
{
    auto [id, nbr_frame, sprite_pos, animation_time] = csprite;
    auto [clock] = ctimer;
    auto textureWidth = (float) assetsLoader.get_asset_from_id(id).width;

    if (clock < animation_time) {
        return;
    }
    clock -= animation_time;
    if (sprite_pos < textureWidth) {
        sprite_pos += textureWidth / nbr_frame;
    } else {
        sprite_pos = 0;
    }
}

} // namespace S
} // namespace ECS