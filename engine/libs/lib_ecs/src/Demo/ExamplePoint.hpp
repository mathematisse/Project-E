/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for ExamplePoint entity declarations
*/

#pragma once

#include "Components/PureComponentPools.hpp"
#include "Entities/AEntityPool.hpp"
#include "Entities/AEntityRef.hpp"

namespace ECS::Components {
DECLARE_COMPONENT(Position, float, float);
DECLARE_COMPONENT(Velocity, float, float);
}// namespace ECS::Components

namespace ECS::Entities {
const size_t ExamplePointChunkSize = 3;

// ENTITY REF
class ExamplePointRef : public AEntityRef
{
public:
  ExamplePointRef(Components::EntityStatusRef *status,
    Components::ChunkPosRef *cPos,
    Components::PositionRef *position,
    Components::VelocityRef *velocity);
  ~ExamplePointRef() override;

  // Default rule of five
  ExamplePointRef(const ExamplePointRef &other) = default;
  ExamplePointRef &operator=(const ExamplePointRef &other) = default;
  ExamplePointRef(ExamplePointRef &&other) = default;
  ExamplePointRef &operator=(ExamplePointRef &&other) = default;

  // Accessors

  [[nodiscard]] Components::PositionRef *getPosition() const;
  void setPosition(const Components::PositionRef &position);

  [[nodiscard]] Components::VelocityRef *getVelocity() const;
  void setVelocity(const Components::VelocityRef &velocity);


protected:
  Components::PositionRef *_position;
  Components::VelocityRef *_velocity;
};

// ENTITY POOL
class ExamplePointPool : public AEntityPool
{
public:
  ExamplePointPool();
  ~ExamplePointPool() override = default;

  // default rule of five
  ExamplePointPool(const ExamplePointPool &other) = default;
  ExamplePointPool &operator=(const ExamplePointPool &other) = default;
  ExamplePointPool(ExamplePointPool &&other) = default;
  ExamplePointPool &operator=(ExamplePointPool &&other) = default;

  std::unique_ptr<Entities::IEntityRef> getEntity(Chunks::ChunkPos cPos) override;
  std::unique_ptr<Entities::ExamplePointRef> getRawEntity(Chunks::ChunkPos cPos);
  std::vector<Components::IComponentPool *> getComponentPools() override;

protected:
  Components::PositionPool _positionPool;
  Components::VelocityPool _velocityPool;
};
}// namespace ECS::Entities
