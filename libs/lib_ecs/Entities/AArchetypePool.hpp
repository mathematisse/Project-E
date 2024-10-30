/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/ComponentConcept.hpp"
#include "lib_ecs/Components/IComponentPool.hpp"
#include "lib_ecs/Entities/IArchetypePool.hpp"

namespace ECS::E {

class AArchetypePool : public IArchetypePool {
public:
    ~AArchetypePool() override = default;
    const std::vector<C::IComponentPool *> &getVecComponentPools() override = 0;
    [[nodiscard]] const char *getEntityName() const override = 0;
    //  std::unique_ptr<IArchetypeRef> getEntity(Chunks::chunkPos_t cPos) override = 0;
    Chunks::cPosArr_t &getFreePos() override = 0;
    Chunks::chunk_idx_t getTotalSize() override = 0;
    Chunks::chunk_idx_t getUsedSize() override = 0;
    [[nodiscard]] Chunks::chunk_idx_t getChunkCount() const override = 0;
    C::IComponentPool *getComponentPool(const std::string &name) override = 0;
    void addChunk() override = 0;

    void resetEntityAtIndex(const Chunks::chunkPos_t &index) override = 0;
    void resetEntityAtIndexes(const Chunks::cPosArr_t &indexes) override = 0;

    C::EntityStatus::Pool getEntityStatusPoolCore() override = 0;
    C::ChunkPos::Pool getChunkPosPoolCore() override = 0;
};

} // namespace ECS::E
