/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntityPools
*/

#include "Entities/PureEntityPools.hpp"
#include "Components/PureComponentPools.hpp"
#include "Entities/PureEntities.hpp"

namespace ECS {
namespace E {

EntityPtrPool::EntityPtrPool():
    AEntityPool("entityPtr", EntityPtrPoolChunkSize)
{
}

std::unique_ptr<IEntityRef> EntityPtrPool::getEntity(Chunks::chunkPos_t cPos) { return getRawEntity(cPos); }

std::unique_ptr<E::EntityPtrRef> EntityPtrPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::EntityPtrRef>(
        dynamic_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
        dynamic_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
        dynamic_cast<C::EntityPoolIdRef *>(_entityPoolIdPool.getComponentRef(cPos))
    );

    return ptr;
}

std::vector<C::IComponentPool *> EntityPtrPool::getComponentPools()
{
    return {&_entityStatusPool, &_chunkPosPool, &_entityPoolIdPool};
}

C::EntityPoolIdPool &EntityPtrPool::getEntityPoolIdPool() { return _entityPoolIdPool; }

} // namespace E
} // namespace ECS
