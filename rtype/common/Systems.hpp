/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "DecorEntities.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "GameEntities.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "raylib.h"

namespace ECS::S {

class MoveBackgroundSystem
    : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool> {
public:
    explicit MoveBackgroundSystem();
    ~MoveBackgroundSystem() override = default;

    float cameraX = 0;

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

    std::vector<Vector2> playersPos;

protected:
    void _statusOperate(
        typename C::EntityStatusPool::Types &cEntityStatus,
        typename C::PositionPool::Types &cposition, typename C::TypePool::Types &ctype
    ) override;
};

} // namespace ECS::S
