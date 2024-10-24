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
class EntityPtrRef : public AEntityRef, public EntityWithEntityPoolIdRef {
public:
    EntityPtrRef(const AEntityRef &aEnt, C::EntityPoolIdRef poolId);
    ~EntityPtrRef() override = default;
    [[nodiscard]] C::entity_pool_id_t getPoolIdElem() const;
    void setPoolIdElem(C::entity_pool_id_t poolId);
};
} // namespace ECS::E
