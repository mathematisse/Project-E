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

class MoveBackgroundSystem
    : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool> {
public:
    explicit MoveBackgroundSystem();
    ~MoveBackgroundSystem() override = default;

    MoveBackgroundSystem(const MoveBackgroundSystem &other) = default;
    MoveBackgroundSystem(MoveBackgroundSystem &&other) = default;
    MoveBackgroundSystem &operator=(const MoveBackgroundSystem &other) = default;
    MoveBackgroundSystem &operator=(MoveBackgroundSystem &&other) = default;

    float cameraX;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::TypePool::Types &ctype
    ) override;
};

class MoveEnnemySystem
    : public S::AStatusMonoSystem<C::PositionPool, C::VelocityPool, C::TypePool> {
public:
    explicit MoveEnnemySystem();
    ~MoveEnnemySystem() override = default;

    MoveEnnemySystem(const MoveEnnemySystem &other) = default;
    MoveEnnemySystem(MoveEnnemySystem &&other) = default;
    MoveEnnemySystem &operator=(const MoveEnnemySystem &other) = default;
    MoveEnnemySystem &operator=(MoveEnnemySystem &&other) = default;

    std::vector<Vector2> playersPos;

protected:
    void _statusOperate(
        C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity,
        C::TypePool::Types &ctype
    ) override;
};

class ColliderSystem
    : public S::ASelfDualSystem<std::tuple<
          C::EntityStatusPool, C::PositionPool, C::SizePool, C::TypePool, C::HealthPool>> {
public:
    explicit ColliderSystem();
    ~ColliderSystem() override = default;

    ColliderSystem(const ColliderSystem &other) = default;
    ColliderSystem(ColliderSystem &&other) = default;
    ColliderSystem &operator=(const ColliderSystem &other) = default;
    ColliderSystem &operator=(ColliderSystem &&other) = default;

protected:
    void _innerOperate(
        C::EntityStatusPool::Types &cStatusA, C::PositionPool::Types &cpositionA,
        C::SizePool::Types &csizeA, C::TypePool::Types &ctypeA, C::HealthPool::Types &chealthA,
        C::EntityStatusPool::Types &cStatusB, C::PositionPool::Types &cpositionB,
        C::SizePool::Types &csizeB, C::TypePool::Types &ctypeB, C::HealthPool::Types &chealthB
    ) override;
};

class GetPlayerPositionSystem
    : public S::AStatusMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool> {
public:
    explicit GetPlayerPositionSystem();
    ~GetPlayerPositionSystem() override = default;

    GetPlayerPositionSystem(const GetPlayerPositionSystem &other) = default;
    GetPlayerPositionSystem(GetPlayerPositionSystem &&other) = default;
    GetPlayerPositionSystem &operator=(const GetPlayerPositionSystem &other) = default;
    GetPlayerPositionSystem &operator=(GetPlayerPositionSystem &&other) = default;

    std::vector<Vector2> playersPos;

protected:
    void _statusOperate(
        typename C::EntityStatusPool::Types &cEntityStatus,
        typename C::PositionPool::Types &cposition, typename C::TypePool::Types &ctype
    ) override;
};

class ChangePlayerWeaponSystem
    : public S::AStatusMonoSystem<C::EntityStatusPool, C::TypePool, C::HealthPool, C::WeaponPool> {
public:
    explicit ChangePlayerWeaponSystem();
    ~ChangePlayerWeaponSystem() override = default;

    ChangePlayerWeaponSystem(const ChangePlayerWeaponSystem &other) = default;
    ChangePlayerWeaponSystem(ChangePlayerWeaponSystem &&other) = default;
    ChangePlayerWeaponSystem &operator=(const ChangePlayerWeaponSystem &other) = default;
    ChangePlayerWeaponSystem &operator=(ChangePlayerWeaponSystem &&other) = default;

protected:
    void _statusOperate(
        typename C::EntityStatusPool::Types &cEntityStatus, typename C::TypePool::Types &ctype,
        typename C::HealthPool::Types &cHealth, typename C::WeaponPool::Types &cweapon
    ) override;
};

} // namespace ECS::S
