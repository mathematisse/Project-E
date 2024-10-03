/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntities
*/

#pragma once

#include "Components/PureComponentPools.hpp"
#include "Entities/AEntityRef.hpp"


namespace ECS::Entities {
class EntityPtrRef : public AEntityRef
{
public:
  EntityPtrRef(Components::EntityStatusRef *status, Components::ChunkPosRef *cPos, Components::EntityPoolIdRef *poolId);
  ~EntityPtrRef() override;
  EntityPtrRef(const EntityPtrRef &other) = default;
  EntityPtrRef(EntityPtrRef &&other) = default;
  EntityPtrRef &operator=(const EntityPtrRef &other) = default;
  EntityPtrRef &operator=(EntityPtrRef &&other) = default;
  [[nodiscard]] Components::entity_pool_id_t getPoolId() const;
  void setPoolId(Components::entity_pool_id_t poolId);

protected:
  Components::EntityPoolIdRef *_poolId;
};
}// namespace ECS::Entities
