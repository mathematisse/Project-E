/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntityPools
*/

#include "lib_ecs/Entities/PureEntityPools.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Entities/PureEntities.hpp"

namespace ECS {
namespace E {

EntityPtrPool::EntityPtrPool():
    AEntityPool("entityPtr", EntityPtrPoolChunkSize)
{
}

std::unique_ptr<IEntityRef> EntityPtrPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::EntityPtrRef> EntityPtrPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::EntityPtrRef>(
        *AEntityPool::getRawEntity(cPos), EntityWithEntityPoolIdPool::getComponentRef(cPos)
    );
    return ptr;
}

std::vector<C::IComponentPool *> EntityPtrPool::getComponentPools()
{
    auto res = AEntityPool::getComponentPools();
    res.push_back(&_EntityPoolIdPool);
    return res;
}

C::EntityPoolIdPool &EntityPtrPool::getEntityPoolIdPool() { return _EntityPoolIdPool; }

} // namespace E
} // namespace ECS
