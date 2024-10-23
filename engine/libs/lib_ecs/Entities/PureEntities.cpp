/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntities
*/

#include "lib_ecs/Entities/PureEntities.hpp"

namespace ECS {
namespace E {
EntityPtrRef::EntityPtrRef(
    const AEntityRef &aEnt, C::EntityPoolIdRef *poolId
):
    AEntityRef(aEnt), EntityWithEntityPoolIdRef(poolId)
{
}

C::entity_pool_id_t EntityPtrRef::getPoolIdVal() const { return *_EntityPoolId->get<0>(); }

void EntityPtrRef::setPoolIdVal(C::entity_pool_id_t poolId) { *_EntityPoolId->get<0>() = poolId; }
} // namespace E
} // namespace ECS
