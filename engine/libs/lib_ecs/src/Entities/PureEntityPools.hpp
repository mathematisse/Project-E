/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "Components/PureComponentPools.hpp"
#include "Entities/AEntityPool.hpp"
#include <memory>


    namespace ECS::Entities
    {
        const size_t EntityPtrPoolChunkSize = 256;


        class EntityPtrPool : public AEntityPool
        {
        public:
            EntityPtrPool();
            ~EntityPtrPool() override;
            EntityPtrPool(const EntityPtrPool &other) = delete;
            EntityPtrPool(EntityPtrPool &&other) = delete;
            EntityPtrPool &operator=(const EntityPtrPool &other) = delete;
            EntityPtrPool &operator=(EntityPtrPool &&other) = delete;
            IEntity* operator[](Chunks::ChunkPos cPos) override;
            std::vector<Components::IComponentPool *> getComponentPools() override;
        protected:
            Components::EntityPoolIdPool _entityPoolIdPool;
        };
    }

