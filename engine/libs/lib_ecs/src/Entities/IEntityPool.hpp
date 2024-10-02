/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntityPool
*/

#pragma once

#include "Entities/IEntity.hpp"
#include "Components/IComponentPool.hpp"
#include <list>
#include <memory>
#include <vector>


    namespace ECS::Entities
    {
        class IEntityPool
        {
        public:
            IEntityPool() = default;
            virtual ~IEntityPool() = default;
            IEntityPool(const IEntityPool &other) = default;
            IEntityPool(IEntityPool &&other) = default;
            IEntityPool &operator=(const IEntityPool &other) = default;
            IEntityPool &operator=(IEntityPool &&other) = default;
            [[nodiscard]] virtual const std::string &getEntityName() const = 0;
            virtual std::unique_ptr<IEntity> getEntity(Chunks::ChunkPos cPos) = 0;
            virtual std::list<Chunks::ChunkPos> &getFreePos() = 0;
            virtual Chunks::chunk_pos_t getTotalSize() = 0;
            virtual Chunks::chunk_pos_t getUsedSize() = 0;
            [[nodiscard]] virtual Chunks::chunk_pos_t getChunkCount() const = 0;
            virtual std::vector<Components::IComponentPool *> getComponentPools() = 0;
            virtual Components::IComponentPool *getComponentPool(const std::string &name) = 0;
            virtual void addChunk() = 0;
        };
    }

