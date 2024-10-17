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
    C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::EntityPoolIdRef *poolId
):
    AEntityRef(status, cPos),
    _poolId(poolId)
{
}

EntityPtrRef::~EntityPtrRef() { delete _poolId; }

C::entity_pool_id_t EntityPtrRef::getPoolId() const { return *_poolId->get<0>(); }

void EntityPtrRef::setPoolId(C::entity_pool_id_t poolId) { *_poolId->get<0>() = poolId; }
} // namespace E
} // namespace ECS
