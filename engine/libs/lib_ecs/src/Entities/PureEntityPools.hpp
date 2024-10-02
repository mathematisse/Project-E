/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "Components/PureComponentPools.hpp"
#include "Entities/AEntityPool.hpp"
#include "Entities/PureEntities.hpp"


    namespace ECS::Entities
    {
        const size_t EntityPtrPoolChunkSize = 256;


        class EntityPtrPool : public AEntityPool
        {
        public:
            EntityPtrPool();
            std::unique_ptr<IEntityRef> getEntity(Chunks::ChunkPos cPos) override;
            std::unique_ptr<Entities::EntityPtrRef> getRawEntity(Chunks::ChunkPos cPos);
            std::vector<Components::IComponentPool *> getComponentPools() override;
        protected:
            Components::EntityPoolIdPool _entityPoolIdPool;
        };

    }

