/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "core/Components.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "AssetsLoader.hpp"
#include "raylib.h"
#include "render/Components.hpp"
#include "spatial2d/Components.hpp"

namespace ECS::S {

class DebugDrawSystem : public S::AStatusMonoSystem<
                            C::Position::Pool, C::Color::Pool, C::Size::Pool, C::RLayer::Pool> {
public:
    DebugDrawSystem(uint8_t layer, Camera2D &camera, bool useCamera);
    ~DebugDrawSystem() override = default;

    uint8_t systemLayer;
    Camera2D &camera;
    bool hasCamera;

protected:
    void _statusOperate(
        typename C::Position::Pool::Types &cposition, typename C::Color::Pool::Types &ccolor,
        typename C::Size::Pool::Types &csize, typename C::RLayer::Pool::Types &clayer
    ) override;
};

class DrawSpriteSystem
    : public S::AMonoSystem<
          C::Position::Pool, C::Size::Pool, C::Rotation::Pool, C::Sprite::Pool, C::RLayer::Pool> {
public:
    DrawSpriteSystem(uint8_t layer, AssetsLoader &assetsLoader, Camera2D &camera, bool useCamera);
    ~DrawSpriteSystem() override = default;

    uint8_t systemLayer;
    AssetsLoader &assetsLoader;
    Camera2D &camera;
    bool hasCamera;

protected:
    void _innerOperate(
        typename C::Position::Pool::Types &cposition, typename C::Size::Pool::Types &csize,
        typename C::Rotation::Pool::Types &crotation, typename C::Sprite::Pool::Types &csprite,
        typename C::RLayer::Pool::Types &clayer
    ) override;
};

class DrawAnimatedSpriteSystem : public S::AMonoSystem<
                                     C::Position::Pool, C::Size::Pool, C::Rotation::Pool,
                                     C::AnimatedSprite::Pool, C::RLayer::Pool> {
public:
    DrawAnimatedSpriteSystem(
        uint8_t layer, AssetsLoader &assetsLoader, Camera2D &camera, bool useCamera
    );
    ~DrawAnimatedSpriteSystem() override = default;

    uint8_t systemLayer;
    AssetsLoader &assetsLoader;
    Camera2D &camera;
    bool hasCamera;

protected:
    void _innerOperate(
        typename C::Position::Pool::Types &cposition, typename C::Size::Pool::Types &csize,
        typename C::Rotation::Pool::Types &crotation,
        typename C::AnimatedSprite::Pool::Types &csprite, typename C::RLayer::Pool::Types &clayer
    ) override;
};

class SpriteAnimationSystem : public S::AMonoSystem<C::AnimatedSprite::Pool, C::Timer::Pool> {
public:
    explicit SpriteAnimationSystem(AssetsLoader &assetsLoader);
    ~SpriteAnimationSystem() override = default;

    AssetsLoader &assetsLoader;

protected:
    void _innerOperate(
        typename C::AnimatedSprite::Pool::Types &csprite, typename C::Timer::Pool::Types &ctimer
    ) override;
};

} // namespace ECS::S
