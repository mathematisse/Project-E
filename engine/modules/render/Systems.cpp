/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "spatial2d/Components.hpp"
#include <raylib.h>

namespace ECS::S {
// SYSTEM

DebugDrawSystem::DebugDrawSystem(Camera2D &camera):
    AStatusMonoSystem(false, C::ENT_ALIVE),
    camera(camera)
{
}

void DebugDrawSystem::_statusOperate(
    C::Position::Pool::Types &cposition, C::Color::Pool::Types &ccolor, C::Size::Pool::Types &csize
)
{
    auto [x, y] = cposition;
    auto [r, g, b, a] = ccolor;
    auto [sizeX, sizeY] = csize;
    BeginMode2D(camera);
    DrawRectangle((int) x, (int) y, (int) sizeX, (int) sizeY, {r, g, b, a});
}

DrawSpriteSystem::DrawSpriteSystem(AssetsLoader &assetsLoader, Camera2D &camera):
    AMonoSystem(false),
    assetsLoader(assetsLoader),
    camera(camera)
{
}

void DrawSpriteSystem::_innerOperate(
    C::Position::Pool::Types &cposition, C::Size::Pool::Types &csize,
    C::Rotation::Pool::Types &crotation, C::Sprite::Pool::Types &csprite
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
        texture, {rotation == 90 ? x + sizeX : x, rotation == -90 ? y + sizeY : y}, rotation,
        sizeX / (float) texture.width, WHITE
    );
}

DrawAnimatedSpriteSystem::DrawAnimatedSpriteSystem(AssetsLoader &assetsLoader, Camera2D &camera):
    AMonoSystem(false),
    assetsLoader(assetsLoader),
    camera(camera)
{
}

void DrawAnimatedSpriteSystem::_innerOperate(
    C::Position::Pool::Types &cposition, C::Size::Pool::Types &csize,
    C::Rotation::Pool::Types &crotation, C::AnimatedSprite::Pool::Types &csprite
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
        {start_position, 0, (float) texture.width / (float) nbr_frame, (float) texture.height},
        {x, rotation == 90 ? y : y + sizeY, sizeX, sizeY}, Vector2 {0, 0}, rotation, WHITE
    );
}

SpriteAnimationSystem::SpriteAnimationSystem(AssetsLoader &assetsLoader):
    AMonoSystem(false),
    assetsLoader(assetsLoader)
{
}

void SpriteAnimationSystem::_innerOperate(
    C::AnimatedSprite::Pool::Types &csprite, C::Timer::Pool::Types &ctimer
)
{
    auto [id, nbr_frame, sprite_pos, animation_time] = csprite;
    auto [clock] = ctimer;
    auto textureWidth = assetsLoader.get_asset_from_id(id).width;

    if (id == 0 || clock < animation_time) {
        return;
    }
    clock -= animation_time;
    if (sprite_pos < (float) textureWidth) {
        sprite_pos += (float) textureWidth / (float) nbr_frame;
    } else {
        sprite_pos = 0;
    }
}

} // namespace ECS::S
