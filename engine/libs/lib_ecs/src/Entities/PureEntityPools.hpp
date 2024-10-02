/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "Components/PureComponentPools.hpp"
#include "Entities/AEntityPool.hpp"
#include "Entities/IEntity.hpp"
#include "Entities/PureEntities.hpp"


    namespace ECS::Entities
    {
        const size_t EntityPtrPoolChunkSize = 256;


        class EntityPtrPool : public AEntityPool
        {
        public:
            EntityPtrPool();
            ~EntityPtrPool() override;
            EntityPtrPool(const EntityPtrPool &other) = default;
            EntityPtrPool(EntityPtrPool &&other) = default;
            EntityPtrPool &operator=(const EntityPtrPool &other) = default;
            EntityPtrPool &operator=(EntityPtrPool &&other) = default;
            std::unique_ptr<IEntity> getEntity(Chunks::ChunkPos cPos) override;
            std::unique_ptr<Entities::EntityPtr> getRawEntity(Chunks::ChunkPos cPos);
            std::vector<Components::IComponentPool *> getComponentPools() override;
        protected:
            Components::EntityPoolIdPool _entityPoolIdPool;
        };
    }

