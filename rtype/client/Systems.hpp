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

class CountEnnemyAliveSystem : public S::AMonoSystem<C::EntityStatusPool, C::TypePool> {
public:
    explicit CountEnnemyAliveSystem(size_t &ennemyCount);
    ~CountEnnemyAliveSystem() override = default;

    CountEnnemyAliveSystem(const CountEnnemyAliveSystem &other) = default;
    CountEnnemyAliveSystem(CountEnnemyAliveSystem &&other) = default;
    CountEnnemyAliveSystem &operator=(const CountEnnemyAliveSystem &other) = default;
    CountEnnemyAliveSystem &operator=(CountEnnemyAliveSystem &&other) = default;

    size_t ennemyCount = 0;

protected:
    void _innerOperate(C::EntityStatusPool::Types &cStatus, C::TypePool::Types &ctype) override;
};

class SpawnEnnemySystem : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool> {
public:
    explicit SpawnEnnemySystem(
        EntityManager &entityManager, NetworkManager &networkManager, AssetsLoader &assetsLoader,
        Camera2D &camera, size_t maxEnnemyCount = 5
    );
    ~SpawnEnnemySystem() override = default;

    SpawnEnnemySystem(const SpawnEnnemySystem &other) = default;
    SpawnEnnemySystem(SpawnEnnemySystem &&other) = default;
    SpawnEnnemySystem &operator=(const SpawnEnnemySystem &other) = default;
    SpawnEnnemySystem &operator=(SpawnEnnemySystem &&other) = default;

    EntityManager &entityManager;
    NetworkManager &networkManager;
    AssetsLoader &assetsLoader;
    Camera2D &camera;
    size_t ennemyCount = 0;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::TypePool::Types &ctype
    ) override;

private:
    size_t _maxEnnemyCount = 0;
};

class ShootSystem
    : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool, C::CanShootPool> {
public:
    explicit ShootSystem(
        EntityManager &entityManager, NetworkManager &networkManager, AssetsLoader &assetsLoader
    );
    ~ShootSystem() override = default;

    ShootSystem(const ShootSystem &other) = default;
    ShootSystem(ShootSystem &&other) = default;
    ShootSystem &operator=(const ShootSystem &other) = default;
    ShootSystem &operator=(ShootSystem &&other) = default;

    EntityManager &entityManager;
    NetworkManager &networkManager;
    AssetsLoader &assetsLoader;
    float deltaTime = 0.0f;
    Vector2 playerPosition;

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

class MoveEnnemySystem
    : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::VelocityPool, C::TypePool> {
public:
    explicit MoveEnnemySystem();
    ~MoveEnnemySystem() override = default;

    MoveEnnemySystem(const MoveEnnemySystem &other) = default;
    MoveEnnemySystem(MoveEnnemySystem &&other) = default;
    MoveEnnemySystem &operator=(const MoveEnnemySystem &other) = default;
    MoveEnnemySystem &operator=(MoveEnnemySystem &&other) = default;

    Vector2 playerPosition;

protected:
    void _innerOperate(
        C::EntityStatusPool::Types &cStatus, C::PositionPool::Types &cposition,
        C::VelocityPool::Types &cvelocity, C::TypePool::Types &ctype
    ) override;
};

class ColliderSystem
    : public S::ASelfDualSystem<
          std::tuple<C::EntityStatusPool, C::PositionPool, C::SizePool, C::TypePool, C::HealthPool>> {
public:
    explicit ColliderSystem();
    ~ColliderSystem() override = default;

    ColliderSystem(const ColliderSystem &other) = default;
    ColliderSystem(ColliderSystem &&other) = default;
    ColliderSystem &operator=(const ColliderSystem &other) = default;
    ColliderSystem &operator=(ColliderSystem &&other) = default;

protected:
    void _innerOperate(
        C::EntityStatusPool::Types &cStatusA, C::PositionPool::Types &cpositionA, C::SizePool::Types &csizeA,
        C::TypePool::Types &ctypeA, C::HealthPool::Types &chealthA, C::EntityStatusPool::Types &cStatusB,
        C::PositionPool::Types &cpositionB, C::SizePool::Types &csizeB, C::TypePool::Types &ctypeB,
        C::HealthPool::Types &chealthB
    ) override;
};

class ClockSystem : public S::AMonoSystem<C::SpritePool, C::TimerPool> {
public:
    explicit ClockSystem(AssetsLoader &assetsLoader);
    ~ClockSystem() override = default;

    ClockSystem(const ClockSystem &other) = default;
    ClockSystem(ClockSystem &&other) = default;
    ClockSystem &operator=(const ClockSystem &other) = default;
    ClockSystem &operator=(ClockSystem &&other) = default;

    AssetsLoader &assetsLoader;
    float deltaTime = 0.0f;

protected:
    void _innerOperate(typename C::SpritePool::Types &csprite, typename C::TimerPool::Types &ctimer) override;
};

class UpdateEnginePosition : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool> {
public:
    explicit UpdateEnginePosition();
    ~UpdateEnginePosition() override = default;

    UpdateEnginePosition(const UpdateEnginePosition &other) = default;
    UpdateEnginePosition(UpdateEnginePosition &&other) = default;
    UpdateEnginePosition &operator=(const UpdateEnginePosition &other) = default;
    UpdateEnginePosition &operator=(UpdateEnginePosition &&other) = default;

    Vector2 playerPosition;
    char playerAlive;

protected:
    void _innerOperate(
        C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition, C::TypePool::Types &ctype
    ) override;
};

} // namespace ECS::S
