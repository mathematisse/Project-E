/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Core/RawComponents.hpp"
#include "lib_log/log.hpp"
#include <string>
#include <tuple>
#include <ranges> // Include for C++20 ranges

#define BLUE "\033[34m"
#define RED "\033[31m"
#define RESET "\033[0m"

namespace ECS {

EntityManager::EntityManager(float fixedUpdateTime):
    AEntityManager(fixedUpdateTime),
    _entityPtrPool(ENTITY_PTR_POOL_SIZE)
{
}

auto EntityManager::_createEntities(
    std::tuple<ECS::E::IArchetypePool *, C::entity_pool_id_t> ePool, size_t count,
    C::EntityStatusEnum status
)
{
    auto [entityPool, poolId] = ePool;

    auto &freePos = entityPool->getFreePos();
    auto &freePtrPos = _entityPtrPool.getFreePos();

    while (freePos.size() < count) {
        entityPool->addChunk();
    }
    while (freePtrPos.size() < count) {
        _entityPtrPool.addChunk();
    }

    // Create views over the first 'count' elements of freePos and freePtrPos
    auto nextFreePosRange = freePos | std::views::take(count);
    auto nextFreePtrPosRange = freePtrPos | std::views::take(count);

    entityPool->getEntityStatusPoolCore().setComponentAtIndexes(nextFreePosRange, status);
    entityPool->getChunkPosPoolCore().setComponentsAtIndexes(nextFreePosRange, nextFreePtrPosRange);

    _entityPtrPool.getEntityStatusPool().setComponentAtIndexes(nextFreePtrPosRange, C::ENT_ALIVE);
    _entityPtrPool.getChunkPosPool().setComponentsAtIndexes(nextFreePtrPosRange, nextFreePosRange);
    _entityPtrPool.getEntityPoolIdPool().setComponentAtIndexes(nextFreePtrPosRange, poolId);

    // Erase used positions from free lists
    freePtrPos.erase(freePtrPos.begin(), freePtrPos.begin() + static_cast<long>(count));
    freePos.erase(freePos.begin(), freePos.begin() + static_cast<long>(count));

    return nextFreePtrPosRange;
}

void EntityManager::destroyEntity(const Chunks::chunkPos_t &cPos)
{
    // get pool
    auto *entityPool = _entityPools[_entityPtrPool.getEntityPoolIdVal(cPos)];
    // span
    std::span<const Chunks::chunkPos_t> cPosArr(&cPos, 1);
    // destroy
    _destroyEntities(cPosArr, entityPool);
}

void EntityManager::destroyEntities(
    std::span<Chunks::chunkPos_t> cPosArr, const std::string &entityName
)
{
    for (auto &entityPool : _entityPools) {
        if (entityPool->getEntityName() == entityName) {
            _destroyEntities(cPosArr, entityPool);
            return;
        }
    }
}

void EntityManager::destroyEntities(std::ranges::input_range auto &&cPosRange)
{
    size_t count = std::ranges::distance(cPosRange);
    LOG_DEBUG(RED "Destroying " + std::to_string(count) + " entities" RESET);

    _entityPtrPool.getEntityStatusPool().setComponentAtIndexes(cPosRange, C::ENT_NONE);
    _entityPtrPool.getFreePos().insert(
        _entityPtrPool.getFreePos().end(), std::ranges::begin(cPosRange),
        std::ranges::end(cPosRange)
    );

    Chunks::cPosArr_t deletedcPos(count);
    _entityPtrPool.getChunkPosPool().getComponentsAtIndexes(cPosRange, deletedcPos);

    std::vector<std::tuple<C::entity_pool_id_t>> poolIds(count);
    _entityPtrPool.getEntityPoolIdPool().getComponentsAtIndexes(cPosRange, poolIds);

    for (size_t i = 0; i < count; ++i) {
        auto [poolId] = poolIds[i];
        auto entCPos = deletedcPos[i];
        _entityPools[poolId]->resetEntityAtIndex(entCPos);
        _entityPools[poolId]->getFreePos().push_back(entCPos);
    }
}

void EntityManager::_destroyEntities(
    std::ranges::input_range auto &&cPosRange, ECS::E::IArchetypePool *entityPool
)
{
    size_t count = std::ranges::distance(cPosRange);
    LOG_DEBUG(RED "Destroying " + std::to_string(count) + " entities" RESET);

    _entityPtrPool.getEntityStatusPool().setComponentAtIndexes(cPosRange, C::ENT_NONE);
    _entityPtrPool.getFreePos().insert(
        _entityPtrPool.getFreePos().end(), std::ranges::begin(cPosRange),
        std::ranges::end(cPosRange)
    );

    Chunks::cPosArr_t deletedcPos(count);
    _entityPtrPool.getChunkPosPool().getComponentsAtIndexes(cPosRange, deletedcPos);

    entityPool->resetEntityAtIndexes(deletedcPos);
    entityPool->getFreePos().insert(
        entityPool->getFreePos().end(), deletedcPos.begin(), deletedcPos.end()
    );
}

} // namespace ECS
