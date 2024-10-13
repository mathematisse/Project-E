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
#include "NetworkManager.hpp"
#include "raylib.h"

namespace ECS::S {

class ApplyVelocitySystem : public S::AStatusMonoSystem<C::PositionPool, C::VelocityPool> {
public:
    explicit ApplyVelocitySystem();
    ~ApplyVelocitySystem() override = default;

    ApplyVelocitySystem(const ApplyVelocitySystem &other) = default;
    ApplyVelocitySystem(ApplyVelocitySystem &&other) = default;
    ApplyVelocitySystem &operator=(const ApplyVelocitySystem &other) = default;
    ApplyVelocitySystem &operator=(ApplyVelocitySystem &&other) = default;

    float deltaTime = 0.0f;

protected:
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::VelocityPool::Types &cvelocity
    ) override;
};

class CountEnnemyAliveSystem : public S::AStatusMonoSystem<C::TypePool> {
public:
    explicit CountEnnemyAliveSystem(size_t &ennemyCount);
    ~CountEnnemyAliveSystem() override = default;

    CountEnnemyAliveSystem(const CountEnnemyAliveSystem &other) = default;
    CountEnnemyAliveSystem(CountEnnemyAliveSystem &&other) = default;
    CountEnnemyAliveSystem &operator=(const CountEnnemyAliveSystem &other) = default;
    CountEnnemyAliveSystem &operator=(CountEnnemyAliveSystem &&other) = default;

    size_t ennemyCount = 0;

protected:
    void _statusOperate(C::TypePool::Types &ctype) override;
};

class SpawnEnnemySystem : public S::AStatusMonoSystem<C::PositionPool, C::TypePool> {
public:
    explicit SpawnEnnemySystem(
        EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId, Camera2D &camera,
        size_t maxEnnemyCount = 5
    );
    ~SpawnEnnemySystem() override = default;

    SpawnEnnemySystem(const SpawnEnnemySystem &other) = default;
    SpawnEnnemySystem(SpawnEnnemySystem &&other) = default;
    SpawnEnnemySystem &operator=(const SpawnEnnemySystem &other) = default;
    SpawnEnnemySystem &operator=(SpawnEnnemySystem &&other) = default;

    EntityManager &entityManager;
    NetworkManager &networkManager;
    Camera2D &camera;
    size_t ennemyCount = 0;

protected:
    size_t _spriteId = 0;
    void
    _statusOperate(typename C::PositionPool::Types &cposition, typename C::TypePool::Types &ctype) override;

private:
    size_t _maxEnnemyCount = 0;
};

class DestroyEntitiesSystem : public S::AStatusMonoSystem<C::ChunkPosPool> {
public:
    explicit DestroyEntitiesSystem(EntityManager &entityManager);
    ~DestroyEntitiesSystem() override = default;

    DestroyEntitiesSystem(const DestroyEntitiesSystem &other) = default;
    DestroyEntitiesSystem(DestroyEntitiesSystem &&other) = default;
    DestroyEntitiesSystem &operator=(const DestroyEntitiesSystem &other) = default;
    DestroyEntitiesSystem &operator=(DestroyEntitiesSystem &&other) = default;

    EntityManager &entityManager;

protected:
    void _statusOperate(typename C::ChunkPosPool::Types &cchunkpos) override;
};

class ShootSystem : public S::AStatusMonoSystem<C::PositionPool, C::TypePool, C::CanShootPool> {
public:
    explicit ShootSystem(EntityManager &entityManager, NetworkManager &networkManager, size_t spriteId);
    ~ShootSystem() override = default;

    ShootSystem(const ShootSystem &other) = default;
    ShootSystem(ShootSystem &&other) = default;
    ShootSystem &operator=(const ShootSystem &other) = default;
    ShootSystem &operator=(ShootSystem &&other) = default;

    EntityManager &entityManager;
    NetworkManager &networkManager;
    float deltaTime = 0.0f;
    Vector2 playerPosition;

protected:
    size_t _spriteId = 0;
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::TypePool::Types &ctype,
        typename C::CanShootPool::Types &canshoot
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

class MoveEnnemySystem : public S::AStatusMonoSystem<C::PositionPool, C::VelocityPool, C::TypePool> {
public:
    explicit MoveEnnemySystem();
    ~MoveEnnemySystem() override = default;

    MoveEnnemySystem(const MoveEnnemySystem &other) = default;
    MoveEnnemySystem(MoveEnnemySystem &&other) = default;
    MoveEnnemySystem &operator=(const MoveEnnemySystem &other) = default;
    MoveEnnemySystem &operator=(MoveEnnemySystem &&other) = default;

    Vector2 playerPosition;

protected:
    void _statusOperate(
        C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity, C::TypePool::Types &ctype
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

} // namespace ECS::S
