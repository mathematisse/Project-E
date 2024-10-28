/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Components.hpp"
#include "lib_ecs/Core/RawComponents.hpp"
#include "spatial2d/Components.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "raylib.h"

namespace ECS::S {

class MoveBackgroundSystem
    : public S::AMonoSystem<C::EntityStatus::Pool, C::Position::Pool, C::Type::Pool> {
public:
    explicit MoveBackgroundSystem();
    ~MoveBackgroundSystem() override = default;

    float cameraX = 0;

protected:
    void _innerOperate(
        typename C::EntityStatus::Pool::Types &cstatus,
        typename C::Position::Pool::Types &cposition, typename C::Type::Pool::Types &ctype
    ) override;
};

class MoveEnnemySystem
    : public S::AMonoSystem<C::Position::Pool, C::Velocity::Pool, C::Type::Pool> {
public:
    explicit MoveEnnemySystem();
    ~MoveEnnemySystem() override = default;

    std::vector<Vector2> playersPos;

protected:
    void _innerOperate(
        C::Position::Pool::Types &cposition, C::Velocity::Pool::Types &cvelocity,
        C::Type::Pool::Types &ctype
    ) override;
};

class ColliderSystem : public S::ASelfDualSystem<std::tuple<
                           C::EntityStatus::Pool, C::Position::Pool, C::Size::Pool, C::Type::Pool,
                           C::Health::Pool>> {
public:
    explicit ColliderSystem();
    ~ColliderSystem() override = default;

protected:
    void _innerOperate(
        C::EntityStatus::Pool::Types &cStatusA, C::Position::Pool::Types &cpositionA,
        C::Size::Pool::Types &csizeA, C::Type::Pool::Types &ctypeA,
        C::Health::Pool::Types &chealthA, C::EntityStatus::Pool::Types &cStatusB,
        C::Position::Pool::Types &cpositionB, C::Size::Pool::Types &csizeB,
        C::Type::Pool::Types &ctypeB, C::Health::Pool::Types &chealthB
    ) override;
};

class GetPlayerPositionSystem
    : public S::AMonoSystem<C::EntityStatus::Pool, C::Position::Pool, C::Type::Pool> {
public:
    explicit GetPlayerPositionSystem();
    ~GetPlayerPositionSystem() override = default;

    std::vector<Vector2> playersPos;

protected:
    void _innerOperate(
        typename C::EntityStatus::Pool::Types &cEntityStatus,
        typename C::Position::Pool::Types &cposition, typename C::Type::Pool::Types &ctype
    ) override;
};

} // namespace ECS::S
