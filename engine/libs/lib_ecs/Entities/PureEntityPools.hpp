/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"
#include "lib_ecs/Entities/PureEntities.hpp"

namespace ECS::E {
const size_t EntityPtrPoolChunkSize = 256;

class EntityPtrPool : public AEntityPool, public EntityWithEntityPoolIdPool {
public:
    EntityPtrPool();
    std::unique_ptr<IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    E::EntityPtrRef getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;

    C::EntityPoolIdPool &getEntityPoolIdPool();
};

} // namespace ECS::E
