/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for AStaticEntity entity declarations
*/

#include "AStaticEntity.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
AStaticEntityRef::AStaticEntityRef(
    const AEntityRef &ent, C::PositionRef *position, C::RotationRef *rotation

):
    AEntityRef(ent),
    EntityWithPositionRef(position),
    EntityWithRotationRef(rotation)
{
}

// ENTITY POOL
AStaticEntityPool::AStaticEntityPool():
    AEntityPool("", 0)
{
}

std::unique_ptr<E::IEntityRef> AStaticEntityPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::AStaticEntityRef> AStaticEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::AStaticEntityRef>(
        *AEntityPool::getRawEntity(cPos), EntityWithPositionPool::getComponentRef(cPos),
        EntityWithRotationPool::getComponentRef(cPos)
    );
    return ptr;
}

std::vector<C::IComponentPool *> AStaticEntityPool::getComponentPools()
{
    return {
        &EntityWithPositionPool::getComponentPool(), &EntityWithRotationPool::getComponentPool()
    };
}
}
