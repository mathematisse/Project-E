/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntityPools
*/

#include "Entities/PureEntityPools.hpp"
#include "Components/PureComponentPools.hpp"
#include "Entities/PureEntities.hpp"


namespace ECS { namespace Entities {

EntityPtrPool::EntityPtrPool() : AEntityPool("entityPtr", EntityPtrPoolChunkSize) {}

std::unique_ptr<IEntityRef> EntityPtrPool::getEntity(Chunks::ChunkPos cPos) { return getRawEntity(cPos); }

std::unique_ptr<Entities::EntityPtrRef> EntityPtrPool::getRawEntity(Chunks::ChunkPos cPos)
{
  auto ptr = std::make_unique<Entities::EntityPtrRef>(
    dynamic_cast<Components::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
    dynamic_cast<Components::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
    dynamic_cast<Components::EntityPoolIdRef *>(_entityPoolIdPool.getComponentRef(cPos)));

  return ptr;
}

std::vector<Components::IComponentPool *> EntityPtrPool::getComponentPools()
{
  return { &_entityStatusPool, &_chunkPosPool, &_entityPoolIdPool };
}

} }// namespace ECS::Entities
