/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for ExamplePoint entity declarations
*/

#include "lib_ecs/Demo/ExamplePoint.hpp"

// ENTITY
namespace ECS {
namespace E {

// ENTITY REF
ExamplePointRef::ExamplePointRef(
    C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *Position, C::VelocityRef *Velocity
):
    AEntityRef(status, cPos),
    _position(Position),
    _velocity(Velocity)
{
}

ExamplePointRef::~ExamplePointRef()
{
    delete _position;
    delete _velocity;
}

[[nodiscard]] C::PositionRef *E::ExamplePointRef::getPosition() const { return _position; }

void E::ExamplePointRef::setPosition(const C::PositionRef &position) { *_position = position; }

[[nodiscard]] C::VelocityRef *E::ExamplePointRef::getVelocity() const { return _velocity; }

void E::ExamplePointRef::setVelocity(const C::VelocityRef &velocity) { *_velocity = velocity; }

// ENTITY POOL
ExamplePointPool::ExamplePointPool():
    AEntityPool("ExamplePoint", ExamplePointChunkSize)
{
}

std::unique_ptr<E::IEntityRef> ExamplePointPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::ExamplePointRef> ExamplePointPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::ExamplePointRef>(
        dynamic_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
        dynamic_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
        dynamic_cast<C::PositionRef *>(_positionPool.getComponentRef(cPos)),
        dynamic_cast<C::VelocityRef *>(_velocityPool.getComponentRef(cPos))
    );
    return ptr;
}

std::vector<C::IComponentPool *> ExamplePointPool::getComponentPools()
{
    return {&_entityStatusPool, &_chunkPosPool, &_positionPool, &_velocityPool};
}
} // namespace E
} // namespace ECS
