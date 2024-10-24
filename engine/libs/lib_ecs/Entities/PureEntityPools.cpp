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
#include "lib_ecs/Entities/IEntityRef.hpp"
#include "lib_ecs/Entities/PureEntities.hpp"
#include <memory>

namespace ECS {
namespace E {

EntityPtrPool::EntityPtrPool():
    AEntityPool("entityPtr", EntityPtrPoolChunkSize)
{
}

std::unique_ptr<IEntityRef> EntityPtrPool::getEntity(Chunks::chunkPos_t cPos)
{
    return std::make_unique<EntityPtrRef>(getRawEntity(cPos));
}

E::EntityPtrRef EntityPtrPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    return {AEntityPool::getRawEntity(cPos), EntityWithEntityPoolIdPool::getComponentRef(cPos)};
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
