/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Core/RawComponents.hpp"
#include "lib_ecs/Components/IComponentPool.hpp"
#include <string>

namespace ECS::E {

class IArchetypePool {
public:
    virtual ~IArchetypePool() = default;
    [[nodiscard]] virtual const char *getEntityName() const = 0;
    virtual Chunks::cPosArr_t &getFreePos() = 0;
    virtual Chunks::chunk_idx_t getTotalSize() = 0;
    virtual Chunks::chunk_idx_t getUsedSize() = 0;
    [[nodiscard]] virtual Chunks::chunk_idx_t getChunkCount() const = 0;
    virtual C::IComponentPool *getComponentPool(const std::string &name) = 0;
    virtual const std::vector<C::IComponentPool *> &getVecComponentPools() = 0;
    virtual void addChunk() = 0;
    virtual void resetEntityAtIndex(const Chunks::chunkPos_t &index) = 0;
    virtual void resetEntityAtIndexes(const Chunks::cPosArr_t &indexes) = 0;

    virtual C::EntityStatus::Pool getEntityStatusPoolCore() = 0;
    virtual C::ChunkPos::Pool getChunkPosPoolCore() = 0;

    virtual void deleteEverything() = 0;
};

} // namespace ECS::E