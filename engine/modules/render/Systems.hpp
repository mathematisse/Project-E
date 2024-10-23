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

class DebugDrawSystem
    : public S::AStatusMonoSystem<C::PositionPool, C::ColorPool, C::SizePool> {
public:
    explicit DebugDrawSystem(Camera2D &camera);
    ~DebugDrawSystem() override = default;


    Camera2D &camera;

protected:
    void _statusOperate(
        typename C::PositionPool::Types &cposition,
        typename C::ColorPool::Types &ccolor, typename C::SizePool::Types &csize
    ) override;
};

class DrawSpriteSystem
    : public S::AStatusMonoSystem<
          C::PositionPool, C::SizePool, C::RotationPool, C::SpritePool> {
public:
    explicit DrawSpriteSystem(AssetsLoader &assetsLoader, Camera2D &camera);
    ~DrawSpriteSystem() override = default;


    AssetsLoader &assetsLoader;
    Camera2D &camera;

protected:
    void _statusOperate(
        typename C::PositionPool::Types &cposition,
        typename C::SizePool::Types &csize, typename C::RotationPool::Types &crotation,
        typename C::SpritePool::Types &csprite
    ) override;
};

class DrawAnimatedSpriteSystem
    : public S::AStatusMonoSystem<
          C::PositionPool, C::SizePool, C::RotationPool, C::AnimatedSpritePool> {
public:
    DrawAnimatedSpriteSystem(AssetsLoader &assetsLoader, Camera2D &camera);
    ~DrawAnimatedSpriteSystem() override = default;


    AssetsLoader &assetsLoader;
    Camera2D &camera;

protected:
    void _statusOperate(
        typename C::PositionPool::Types &cposition,
        typename C::SizePool::Types &csize, typename C::RotationPool::Types &crotation,
        typename C::AnimatedSpritePool::Types &csprite
    ) override;
};

class SpriteAnimationSystem : public S::AStatusMonoSystem<C::AnimatedSpritePool, C::TimerPool> {
public:
    explicit SpriteAnimationSystem(AssetsLoader &assetsLoader);
    ~SpriteAnimationSystem() override = default;


    AssetsLoader &assetsLoader;

protected:
    void _statusOperate(
        typename C::AnimatedSpritePool::Types &csprite, typename C::TimerPool::Types &ctimer
    ) override;
};

} // namespace ECS::S
