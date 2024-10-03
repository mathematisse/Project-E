/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for ExamplePoint entity declarations
*/

#include "ExamplePoint.hpp"

// ENTITY
namespace ECS { namespace Entities {

// ENTITY REF
ExamplePointRef::ExamplePointRef(Components::EntityStatusRef *status,
  Components::ChunkPosRef *cPos,
  Components::PositionRef *Position,
  Components::VelocityRef *Velocity)
  : AEntityRef(status, cPos), _position(Position), _velocity(Velocity)
{}

ExamplePointRef::~ExamplePointRef()
{
  delete _position;
  delete _velocity;
}


[[nodiscard]] Components::PositionRef *Entities::ExamplePointRef::getPosition() const { return _position; }

void Entities::ExamplePointRef::setPosition(const Components::PositionRef &position) { *_position = position; }

[[nodiscard]] Components::VelocityRef *Entities::ExamplePointRef::getVelocity() const { return _velocity; }

void Entities::ExamplePointRef::setVelocity(const Components::VelocityRef &velocity) { *_velocity = velocity; }


// ENTITY POOL
ExamplePointPool::ExamplePointPool() : AEntityPool("ExamplePoint", ExamplePointChunkSize) {}

std::unique_ptr<Entities::IEntityRef> ExamplePointPool::getEntity(Chunks::ChunkPos cPos) { return getRawEntity(cPos); }

std::unique_ptr<Entities::ExamplePointRef> ExamplePointPool::getRawEntity(Chunks::ChunkPos cPos)
{
  auto ptr = std::make_unique<Entities::ExamplePointRef>(
    dynamic_cast<Components::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
    dynamic_cast<Components::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
    dynamic_cast<Components::PositionRef *>(_positionPool.getComponentRef(cPos)),
    dynamic_cast<Components::VelocityRef *>(_velocityPool.getComponentRef(cPos)));
  return ptr;
}

std::vector<Components::IComponentPool *> ExamplePointPool::getComponentPools()
{
  return { &_entityStatusPool, &_chunkPosPool, &_positionPool, &_velocityPool };
}
} }// namespace ECS::Entities
