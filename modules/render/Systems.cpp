/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "lib_ecs/Systems/ExecutionTypes.hpp"
#include "spatial2d/Components.hpp"
#include <raylib.h>

namespace ECS::S {
// SYSTEM

DebugDrawSystem::DebugDrawSystem(uint8_t layer, Camera2D &camera, bool useCamera):
    systemLayer(layer),
    camera(camera),
    hasCamera(useCamera)
{
}

void DebugDrawSystem::_statusOperate(
    C::Position::Pool::Types &cposition, C::Color::Pool::Types &ccolor, C::Size::Pool::Types &csize,
    C::RLayer::Pool::Types &clayer
)
{
    auto [layer] = clayer;
    if (layer != this->systemLayer) {
        return;
    }
    auto [x, y] = cposition;
    auto [r, g, b, a] = ccolor;
    auto [sizeX, sizeY] = csize;
    if (hasCamera) {
        BeginMode2D(camera);
    }
    DrawRectangle((int) x, (int) y, (int) sizeX, (int) sizeY, {r, g, b, a});
}

DrawSpriteSystem::DrawSpriteSystem(
    uint8_t layer, AssetsLoader &assetsLoader, Camera2D &camera, bool useCamera
):
    systemLayer(layer),
    assetsLoader(assetsLoader),
    camera(camera),
    hasCamera(useCamera)
{
}

void DrawSpriteSystem::_innerOperate(
    C::Position::Pool::Types &cposition, C::Size::Pool::Types &csize,
    C::Rotation::Pool::Types &crotation, C::Sprite::Pool::Types &csprite,
    C::RLayer::Pool::Types &clayer
)
{
    auto [layer] = clayer;
    if (layer != this->systemLayer) {
        return;
    }
    auto [id] = csprite;
    if (id == 0) {
        return;
    }
    auto [x, y] = cposition;
    auto [sizeX, sizeY] = csize;
    auto [rotation] = crotation;
    auto texture = assetsLoader.get_asset_from_id(id);
    if (hasCamera) {
        BeginMode2D(camera);
    }
    Rectangle source = {0, 0, (float) texture.width, (float) texture.height};
    Rectangle dest = {x, y, sizeX, sizeY};
    Vector2 origin = {sizeX / 2.0F, sizeY / 2.0F}; // centers rot and position
    DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}

DrawAnimatedSpriteSystem::DrawAnimatedSpriteSystem(
    uint8_t layer, AssetsLoader &assetsLoader, Camera2D &camera, bool useCamera
):
    systemLayer(layer),
    assetsLoader(assetsLoader),
    camera(camera),
    hasCamera(useCamera)
{
}

void DrawAnimatedSpriteSystem::_innerOperate(
    C::Position::Pool::Types &cposition, C::Size::Pool::Types &csize,
    C::Rotation::Pool::Types &crotation, C::AnimatedSprite::Pool::Types &csprite,
    C::RLayer::Pool::Types &clayer
)
{
    auto [layer] = clayer;
    if (layer != this->systemLayer) {
        return;
    }
    auto [id, nbr_frame, start_position, _] = csprite;
    if (id == 0) {
        return;
    }
    auto [x, y] = cposition;
    auto [sizeX, sizeY] = csize;
    auto [rotation] = crotation;
    auto texture = assetsLoader.get_asset_from_id(id);
    if (hasCamera) {
        BeginMode2D(camera);
    }
    Rectangle source = {
        start_position, 0, (float) texture.width / (float) nbr_frame, (float) texture.height
    };
    Rectangle dest = {x, y, sizeX, sizeY};
    Vector2 origin = {sizeX / 2.0F, sizeY / 2.0F}; // centers rot and position
    DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}

SpriteAnimationSystem::SpriteAnimationSystem(AssetsLoader &assetsLoader):
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
