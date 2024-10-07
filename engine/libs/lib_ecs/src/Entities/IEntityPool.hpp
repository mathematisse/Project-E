/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntityPool
*/

#pragma once

#include "Components/IComponentPool.hpp"
#include "Components/PureComponentPools.hpp"
#include "Entities/IEntityRef.hpp"
#include <memory>
#include <vector>

namespace ECS::E {
class IEntityPool {
public:
    IEntityPool() = default;
    virtual ~IEntityPool() = default;
    IEntityPool(const IEntityPool &other) = default;
    IEntityPool(IEntityPool &&other) = default;
    IEntityPool &operator=(const IEntityPool &other) = default;
    IEntityPool &operator=(IEntityPool &&other) = default;
    [[nodiscard]] virtual const std::string &getEntityName() const = 0;
    virtual std::unique_ptr<IEntityRef> getEntity(Chunks::chunkPos_t cPos) = 0;
    virtual Chunks::cPosArr_t &getFreePos() = 0;
    virtual Chunks::chunk_idx_t getTotalSize() = 0;
    virtual Chunks::chunk_idx_t getUsedSize() = 0;
    [[nodiscard]] virtual Chunks::chunk_idx_t getChunkCount() const = 0;
    virtual std::vector<C::IComponentPool *> getComponentPools() = 0;
    virtual C::IComponentPool *getComponentPool(const std::string &name) = 0;
    virtual void addChunk() = 0;

    virtual C::EntityStatusPool &getEntityStatusPool() = 0;
    virtual C::ChunkPosPool &getChunkPosPool() = 0;
};
} // namespace ECS::E
