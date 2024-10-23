/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntityPool
*/

#include "lib_ecs/Entities/AEntityPool.hpp"

#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
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

Chunks::chunk_idx_t AEntityPool::getTotalSize()
{
    return EntityWithEntityStatusPool::getComponentPool().elemCount();
}

Chunks::chunk_idx_t AEntityPool::getUsedSize()
{
    return EntityWithEntityStatusPool::getComponentPool().elemCount() - _freePos.size();
}

Chunks::chunk_idx_t AEntityPool::getChunkCount() const
{
    return EntityWithEntityStatusPool::getComponentPool().chunkCount();
}

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

std::unique_ptr<IEntityRef> AEntityPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<AEntityRef> AEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::AEntityRef>(
        EntityWithEntityStatusPool::getComponentRef(cPos),
        EntityWithChunkPosPool::getComponentRef(cPos)
    );
    return ptr;
}

std::vector<C::IComponentPool *> AEntityPool::getComponentPools()
{
    return {
        &EntityWithEntityStatusPool::getComponentPool(), &EntityWithChunkPosPool::getComponentPool()
    };
}

C::EntityStatusPool &AEntityPool::getEntityStatusPool() { return _EntityStatusPool; }

C::ChunkPosPool &AEntityPool::getChunkPosPool() { return _ChunkPosPool; }

} // namespace E
} // namespace ECS
