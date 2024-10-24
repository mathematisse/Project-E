/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntityPool
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/IEntityPool.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"

namespace ECS::E {

class AEntityPool : public IEntityPool,
                    public EntityWithEntityStatusPool,
                    public EntityWithChunkPosPool {
public:
    AEntityPool(std::string entityName, size_t chunkSize);
    ~AEntityPool() override;

    [[nodiscard]] const std::string &getEntityName() const override;
    Chunks::cPosArr_t &getFreePos() override;
    Chunks::chunk_idx_t getTotalSize() override;
    Chunks::chunk_idx_t getUsedSize() override;
    [[nodiscard]] Chunks::chunk_idx_t getChunkCount() const override;
    C::IComponentPool *getComponentPool(const std::string &name) override;
    void addChunk() override;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    E::AEntityRef getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;

    C::EntityStatusPool &getEntityStatusPool() override;
    C::ChunkPosPool &getChunkPosPool() override;

protected:
    std::string _entityName;
    size_t _chunkSize;

    Chunks::cPosArr_t _freePos;
};
} // namespace ECS::E
