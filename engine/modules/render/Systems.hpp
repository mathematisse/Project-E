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

static Camera2D dummyCamera2D = {};

class DebugDrawSystem
    : public S::AStatusMonoSystem<C::Position::Pool, C::Color::Pool, C::Size::Pool> {
public:
    explicit DebugDrawSystem(Camera2D &camera);
    DebugDrawSystem();
    ~DebugDrawSystem() override = default;

    Camera2D &camera;
    bool hasCamera;

protected:
    void _statusOperate(
        typename C::Position::Pool::Types &cposition, typename C::Color::Pool::Types &ccolor,
        typename C::Size::Pool::Types &csize
    ) override;
};

class DrawSpriteSystem
    : public S::AMonoSystem<C::Position::Pool, C::Size::Pool, C::Rotation::Pool, C::Sprite::Pool> {
public:
    explicit DrawSpriteSystem(AssetsLoader &assetsLoader, Camera2D &camera);
    DrawSpriteSystem(AssetsLoader &assetsLoader);
    ~DrawSpriteSystem() override = default;

    AssetsLoader &assetsLoader;
    Camera2D &camera;
    bool hasCamera;

protected:
    void _innerOperate(
        typename C::Position::Pool::Types &cposition, typename C::Size::Pool::Types &csize,
        typename C::Rotation::Pool::Types &crotation, typename C::Sprite::Pool::Types &csprite
    ) override;
};

class DrawAnimatedSpriteSystem
    : public S::AMonoSystem<
          C::Position::Pool, C::Size::Pool, C::Rotation::Pool, C::AnimatedSprite::Pool> {
public:
    DrawAnimatedSpriteSystem(AssetsLoader &assetsLoader, Camera2D &camera);
    DrawAnimatedSpriteSystem(AssetsLoader &assetsLoader);
    ~DrawAnimatedSpriteSystem() override = default;

    AssetsLoader &assetsLoader;
    Camera2D &camera;
    bool hasCamera;

protected:
    void _innerOperate(
        typename C::Position::Pool::Types &cposition, typename C::Size::Pool::Types &csize,
        typename C::Rotation::Pool::Types &crotation,
        typename C::AnimatedSprite::Pool::Types &csprite
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
