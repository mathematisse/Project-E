/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntityPool
*/

#include "lib_ecs/Entities/AEntityPool.hpp"

#include "lib_ecs/Chunks/ChunkPos.hpp"
#include <iostream>
#include <utility>

namespace ECS {
namespace E {
AEntityPool::AEntityPool(std::string entityName, size_t chunkSize):
    _entityName(std::move(entityName)),
    _chunkSize(chunkSize)
{
}

AEntityPool::~AEntityPool() = default;

const std::string &AEntityPool::getEntityName() const { return _entityName; }

Chunks::cPosArr_t &AEntityPool::getFreePos() { return _freePos; }

Chunks::chunk_idx_t AEntityPool::getTotalSize() { return _entityStatusPool.elemCount(); }

Chunks::chunk_idx_t AEntityPool::getUsedSize() { return _entityStatusPool.elemCount() - _freePos.size(); }

Chunks::chunk_idx_t AEntityPool::getChunkCount() const { return _entityStatusPool.chunkCount(); }

C::IComponentPool *AEntityPool::getComponentPool(const std::string &name)
{
    for (auto *componentPool : getComponentPools()) {
        if (componentPool->getComponentName() == name) {
            return componentPool;
        }
    }
    return nullptr;
}

void AEntityPool::addChunk()
{
    std::cout << "Adding chunk to entity pool " << _entityName << '\n';
    auto cCount = getChunkCount();
    for (auto *componentPool : getComponentPools()) {
        componentPool->addChunk(_chunkSize);
    }
    // adds all the free positions of the new chunk
    _freePos.reserve(_freePos.size() + _chunkSize);
    for (size_t i = 0; i < _chunkSize; i++) {
        _freePos.emplace_back(Chunks::ChunkPos {cCount, i});
    }
}

C::EntityStatusPool &AEntityPool::getEntityStatusPool() { return _entityStatusPool; }
C::ChunkPosPool &AEntityPool::getChunkPosPool() { return _chunkPosPool; }
} // namespace E
} // namespace ECS
