/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for ADynamicEntity entity declarations
*/

#include "ADynamicEntity.hpp"
#include "spatial2d/AStaticEntity.hpp"
#include "spatial2d/Components.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
ADynamicEntityRef::ADynamicEntityRef(
    const AStaticEntityRef &staticEnt, C::VelocityRef *velocity, C::AccelerationRef *acceleration
):
    AEntityRef(staticEnt),
    EntityWithPositionRef(staticEnt),
    EntityWithRotationRef(staticEnt),
    AStaticEntityRef(staticEnt),
    EntityWithVelocityRef(velocity),
    EntityWithAccelerationRef(acceleration)
{
}


// ENTITY POOL
ADynamicEntityPool::ADynamicEntityPool():
    AEntityPool("", 0)
{
}

std::unique_ptr<E::IEntityRef> ADynamicEntityPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::ADynamicEntityRef> ADynamicEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::ADynamicEntityRef>(
        *AStaticEntityPool::getRawEntity(cPos),
        EntityWithVelocityPool::getComponentRef(cPos),
        EntityWithAccelerationPool::getComponentRef(cPos)
    );
    return ptr;
}

std::vector<C::IComponentPool *> ADynamicEntityPool::getComponentPools()
{
    return {&EntityWithVelocityPool::getComponentPool(), &EntityWithAccelerationPool::getComponentPool()};
}
}
