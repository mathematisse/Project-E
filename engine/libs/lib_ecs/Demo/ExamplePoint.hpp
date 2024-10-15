/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for ExamplePoint entity declarations
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"

namespace ECS::C {
DECLARE_COMPONENT(Position, float, float);
DECLARE_COMPONENT(Velocity, float, float);
} // namespace ECS::C

namespace ECS::E {
const size_t ExamplePointChunkSize = 3;

// ENTITY REF
class ExamplePointRef : public AEntityRef {
public:
    ExamplePointRef(
        C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *position,
        C::VelocityRef *velocity
    );
    ~ExamplePointRef() override;

    // Default rule of five
    ExamplePointRef(const ExamplePointRef &other) = default;
    ExamplePointRef &operator=(const ExamplePointRef &other) = default;
    ExamplePointRef(ExamplePointRef &&other) = default;
    ExamplePointRef &operator=(ExamplePointRef &&other) = default;

    // Accessors

    [[nodiscard]] C::PositionRef *getPosition() const;
    void setPosition(const C::PositionRef &position);

    [[nodiscard]] C::VelocityRef *getVelocity() const;
    void setVelocity(const C::VelocityRef &velocity);

protected:
    C::PositionRef *_position;
    C::VelocityRef *_velocity;
};

// ENTITY POOL
class ExamplePointPool : public AEntityPool {
public:
    ExamplePointPool();
    ~ExamplePointPool() override = default;

    // default rule of five
    ExamplePointPool(const ExamplePointPool &other) = default;
    ExamplePointPool &operator=(const ExamplePointPool &other) = default;
    ExamplePointPool(ExamplePointPool &&other) = default;
    ExamplePointPool &operator=(ExamplePointPool &&other) = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::ExamplePointRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;

protected:
    C::PositionPool _positionPool;
    C::VelocityPool _velocityPool;
};
} // namespace ECS::E
