/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntityPool
*/

#pragma once

#include "Entities/IEntity.hpp"
#include "Components/IComponentPool.hpp"
#include <cstdint>
#include <list>
#include <vector>



    namespace ECS::Entities
    {
        class IEntityPool
        {
        public:
            IEntityPool() = default;
            virtual ~IEntityPool() = default;
            IEntityPool(const IEntityPool &other) = delete;
            IEntityPool(IEntityPool &&other) = delete;
            IEntityPool &operator=(const IEntityPool &other) = delete;
            IEntityPool &operator=(IEntityPool &&other) = delete;
            [[nodiscard]] virtual const std::string &getEntityName() const = 0;
            virtual IEntity*operator[](Chunks::ChunkPos cPos) = 0;
            virtual std::list<Chunks::ChunkPos> &getFreePos() = 0;
            virtual uint64_t getTotalSize() = 0;
            virtual uint64_t getUsedSize() = 0;
            [[nodiscard]] virtual uint64_t getChunkCount() const = 0;
            virtual std::vector<Components::IComponentPool *> getComponentPools() = 0;
            virtual Components::IComponentPool *getComponentPool(const std::string &name) = 0;
            virtual void addChunk() = 0;
        };
    }

