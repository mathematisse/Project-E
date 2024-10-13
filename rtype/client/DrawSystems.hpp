/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "DecorSquare.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "Square.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "AssetsLoader.hpp"
#include "NetworkManager.hpp"
#include "raylib.h"

namespace ECS::S {

class DrawSystem : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::ColorPool, C::SizePool> {
public:
    explicit DrawSystem(Camera2D &camera);
    ~DrawSystem() override = default;

    DrawSystem(const DrawSystem &other) = default;
    DrawSystem(DrawSystem &&other) = default;
    DrawSystem &operator=(const DrawSystem &other) = default;
    DrawSystem &operator=(DrawSystem &&other) = default;

    Camera2D &camera;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::ColorPool::Types &ccolor, typename C::SizePool::Types &csize
    ) override;
};

class DrawSpriteSystem
    : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::SizePool, C::TypePool, C::SpritePool> {
public:
    explicit DrawSpriteSystem(AssetsLoader &assetsLoader, Camera2D &camera);
    ~DrawSpriteSystem() override = default;

    DrawSpriteSystem(const DrawSpriteSystem &other) = default;
    DrawSpriteSystem(DrawSpriteSystem &&other) = default;
    DrawSpriteSystem &operator=(const DrawSpriteSystem &other) = default;
    DrawSpriteSystem &operator=(DrawSpriteSystem &&other) = default;

    AssetsLoader &assetsLoader;
    Camera2D &camera;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::SizePool::Types &csize, typename C::TypePool::Types &ctype,
        typename C::SpritePool::Types &csprite
    ) override;
};

class ShowInfoSystem : public S::AMonoSystem<C::TypePool, C::HealthPool> {
public:
    explicit ShowInfoSystem(Camera2D &camera);
    ~ShowInfoSystem() override = default;

    ShowInfoSystem(const ShowInfoSystem &other) = default;
    ShowInfoSystem(ShowInfoSystem &&other) = default;
    ShowInfoSystem &operator=(const ShowInfoSystem &other) = default;
    ShowInfoSystem &operator=(ShowInfoSystem &&other) = default;

    Camera2D &camera;
    bool one_time = false;

protected:
    void _innerOperate(C::TypePool::Types &ctype, C::HealthPool::Types &chealth) override;
};

class AnimationSystem : public S::AStatusMonoSystem<C::SpritePool, C::TimerPool> {
public:
    explicit AnimationSystem(AssetsLoader &assetsLoader);
    ~AnimationSystem() override = default;

    AnimationSystem(const AnimationSystem &other) = default;
    AnimationSystem(AnimationSystem &&other) = default;
    AnimationSystem &operator=(const AnimationSystem &other) = default;
    AnimationSystem &operator=(AnimationSystem &&other) = default;

    AssetsLoader &assetsLoader;
    float deltaTime = 0.0f;

protected:
    void
    _statusOperate(typename C::SpritePool::Types &csprite, typename C::TimerPool::Types &ctimer) override;
};

} // namespace ECS::S
