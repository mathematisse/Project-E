/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "Square.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"

namespace ECS::S {

class DrawSystem : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool> {
public:
    explicit DrawSystem();
    ~DrawSystem() override = default;

    DrawSystem(const DrawSystem &other) = default;
    DrawSystem(DrawSystem &&other) = default;
    DrawSystem &operator=(const DrawSystem &other) = default;
    DrawSystem &operator=(DrawSystem &&other) = default;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition
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
    void _innerOperate(
        typename C::PositionPool::Types &cposition, typename C::VelocityPool::Types &cvelocity
    ) override;
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
        typename C::EntityStatusPool::Types &cstatus, typename C::VelocityPool::Types &cvelocity, typename C::TypePool::Types &ctype
    ) override;
};

} // namespace ECS::S
