/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntities
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"

namespace ECS::E {
class EntityPtrRef : public AEntityRef {
public:
    EntityPtrRef(C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::EntityPoolIdRef *poolId);
    ~EntityPtrRef() override;
    EntityPtrRef(const EntityPtrRef &other) = default;
    EntityPtrRef(EntityPtrRef &&other) = default;
    EntityPtrRef &operator=(const EntityPtrRef &other) = default;
    EntityPtrRef &operator=(EntityPtrRef &&other) = default;
    [[nodiscard]] C::entity_pool_id_t getPoolId() const;
    void setPoolId(C::entity_pool_id_t poolId);

protected:
    C::EntityPoolIdRef *_poolId;
};
} // namespace ECS::E
