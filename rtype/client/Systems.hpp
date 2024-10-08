/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "Square.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "AssetsLoader.hpp"
#include "raylib.h"

namespace ECS::S {

class DrawSystem : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::ColorPool, C::SizePool> {
public:
    explicit DrawSystem();
    ~DrawSystem() override = default;

    DrawSystem(const DrawSystem &other) = default;
    DrawSystem(DrawSystem &&other) = default;
    DrawSystem &operator=(const DrawSystem &other) = default;
    DrawSystem &operator=(DrawSystem &&other) = default;

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

class ApplyVelocitySystem : public S::AMonoSystem<C::PositionPool, C::VelocityPool> {
public:
    explicit ApplyVelocitySystem();
    ~ApplyVelocitySystem() override = default;

    ApplyVelocitySystem(const ApplyVelocitySystem &other) = default;
    ApplyVelocitySystem(ApplyVelocitySystem &&other) = default;
    ApplyVelocitySystem &operator=(const ApplyVelocitySystem &other) = default;
    ApplyVelocitySystem &operator=(ApplyVelocitySystem &&other) = default;

    float deltaTime = 0.0f;

protected:
    void _innerOperate(typename C::PositionPool::Types &cposition, typename C::VelocityPool::Types &cvelocity)
        override;
};

class MovePlayerSystem : public S::AMonoSystem<C::EntityStatusPool, C::VelocityPool, C::TypePool> {
public:
    explicit MovePlayerSystem();
    ~MovePlayerSystem() override = default;

    MovePlayerSystem(const MovePlayerSystem &other) = default;
    MovePlayerSystem(MovePlayerSystem &&other) = default;
    MovePlayerSystem &operator=(const MovePlayerSystem &other) = default;
    MovePlayerSystem &operator=(MovePlayerSystem &&other) = default;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::VelocityPool::Types &cvelocity,
        typename C::TypePool::Types &ctype
    ) override;
};

class SpawnEnnemySystem : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool> {
public:
    explicit SpawnEnnemySystem(
        EntityManager &entityManager, AssetsLoader &assetsLoader, Camera2D &camera, size_t maxEnnemyCount = 5
    );
    ~SpawnEnnemySystem() override = default;

    SpawnEnnemySystem(const SpawnEnnemySystem &other) = default;
    SpawnEnnemySystem(SpawnEnnemySystem &&other) = default;
    SpawnEnnemySystem &operator=(const SpawnEnnemySystem &other) = default;
    SpawnEnnemySystem &operator=(SpawnEnnemySystem &&other) = default;

    EntityManager &entityManager;
    AssetsLoader &assetsLoader;
    Camera2D &camera;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::TypePool::Types &ctype
    ) override;

private:
    size_t _ennemyCount = 0;
    size_t _maxEnnemyCount = 0;
};

class ShootSystem
    : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool, C::CanShootPool> {
public:
    explicit ShootSystem(EntityManager &entityManager, AssetsLoader &assetsLoader);
    ~ShootSystem() override = default;

    ShootSystem(const ShootSystem &other) = default;
    ShootSystem(ShootSystem &&other) = default;
    ShootSystem &operator=(const ShootSystem &other) = default;
    ShootSystem &operator=(ShootSystem &&other) = default;

    EntityManager &entityManager;
    AssetsLoader &assetsLoader;
    float deltaTime = 0.0f;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::TypePool::Types &ctype, typename C::CanShootPool::Types &canshoot
    ) override;
};

class MoveBackgroundSystem : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool> {
public:
    explicit MoveBackgroundSystem(Camera2D &camera);
    ~MoveBackgroundSystem() override = default;

    MoveBackgroundSystem(const MoveBackgroundSystem &other) = default;
    MoveBackgroundSystem(MoveBackgroundSystem &&other) = default;
    MoveBackgroundSystem &operator=(const MoveBackgroundSystem &other) = default;
    MoveBackgroundSystem &operator=(MoveBackgroundSystem &&other) = default;

    Camera2D &camera;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::TypePool::Types &ctype
    ) override;
};

} // namespace ECS::S
