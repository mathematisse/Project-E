/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/PureEntities.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include "lib_log/log.hpp"
#include <string>
#include <tuple>

#define BLUE "\033[34m"
#define RED "\033[31m"
#define RESET "\033[0m"

namespace ECS {

EntityManager::EntityManager(float fixedUpdateTime):
    _fixedUpdateTime(fixedUpdateTime)
{
}

bool EntityManager::registerSystemGroup(
    const std::string &targetGroup, const std::string &newGroup, bool addBefore, bool addInside
)
{
    return _systemTree.addSystemGroup(targetGroup, newGroup, addBefore, addInside);
}

bool EntityManager::registerSystem(S::ISystem &system, const std::string &group, bool atStart)
{
    for (auto &entityPool : _entityPools) {
        system.tryAddEntityPool(entityPool);
    }
    return _systemTree.addSystem(&system, group, atStart);
}

bool EntityManager::registerSystemNode(
    S::SystemTreeNode &node, const std::string &targetGroup, bool addBefore, bool addInside
)
{
    for (auto &entityPool : _entityPools) {
        node.registerEntityPool(entityPool);
    }
    return _systemTree.addSystemTreeNode(node, targetGroup, addBefore, addInside);
}

bool EntityManager::registerFixedSystemGroup(
    const std::string &targetGroup, const std::string &newGroup, bool addBefore, bool addInside
)
{
    return _fixedSystemTree.addSystemGroup(targetGroup, newGroup, addBefore, addInside);
}

bool EntityManager::registerFixedSystem(S::ISystem &system, const std::string &group, bool atStart)
{
    for (auto &entityPool : _entityPools) {
        system.tryAddEntityPool(entityPool);
    }
    return _fixedSystemTree.addSystem(&system, group, atStart);
}

bool EntityManager::registerFixedSystemNode(
    S::SystemTreeNode &node, const std::string &targetGroup, bool addBefore, bool addInside
)
{
    for (auto &entityPool : _entityPools) {
        node.registerEntityPool(entityPool);
    }
    return _fixedSystemTree.addSystemTreeNode(node, targetGroup, addBefore, addInside);
}

bool EntityManager::registerEntityPool(E::IEntityPool *entityPool)
{
    _systemTree.registerEntityPool(entityPool);
    _fixedSystemTree.registerEntityPool(entityPool);
    _entityPools.push_back(entityPool);
    return true;
}

S::IQuery &EntityManager::initializeQuery(S::IQuery &query)
{
    LOG_DEBUG(BLUE "Initializing Query" RESET);
    for (auto &entityPool : _entityPools) {
        query.tryAddEntityPool(entityPool);
    }
    return query;
}

std::unique_ptr<E::IEntityRef> EntityManager::getEntity(const E::EntityPtrRef &entityPtr)
{
    return (*(_entityPools[entityPtr.getPoolIdElem()])).getEntity(entityPtr.getChunkPosElem());
}

std::unique_ptr<E::IEntityRef> EntityManager::getEntity(const Chunks::chunkPos_t &cPos)
{
    auto entityPtr = _entityPtrPool.getRawEntity(cPos);
    return (*(_entityPools[entityPtr.getPoolIdElem()])).getEntity(entityPtr.getChunkPosElem());
}

Chunks::cPosArr_t EntityManager::_createEntities(
    std::tuple<ECS::E::IEntityPool *, C::entity_pool_id_t> ePool, size_t count,
    C::EntityStatusEnum status
)
{
    auto [entityPool, poolId] = ePool;

    auto &freePos = entityPool->getFreePos();
    auto &freePtrPos = _entityPtrPool.getFreePos();

    Chunks::cPosArr_t cPosArr; // Results

    while (freePos.size() < count) {
        entityPool->addChunk();
    }
    while (freePtrPos.size() < count) {
        _entityPtrPool.addChunk();
    }

    auto nextFreePosArr = std::vector(freePos.begin(), freePos.begin() + static_cast<long>(count));
    auto nextFreePtrPosArr =
        std::vector(freePtrPos.begin(), freePtrPos.begin() + static_cast<long>(count));

    entityPool->getEntityStatusPool().setComponentAtIndexes(nextFreePosArr, status);
    entityPool->getChunkPosPool().setComponentsAtIndexes(nextFreePosArr, nextFreePtrPosArr);

    _entityPtrPool.getEntityStatusPool().setComponentAtIndexes(nextFreePtrPosArr, C::ENT_ALIVE);
    _entityPtrPool.getChunkPosPool().setComponentsAtIndexes(nextFreePtrPosArr, nextFreePosArr);
    _entityPtrPool.getEntityPoolIdPool().setComponentAtIndexes(nextFreePtrPosArr, poolId);

    freePtrPos.erase(freePtrPos.begin(), freePtrPos.begin() + static_cast<long>(count));
    freePos.erase(freePos.begin(), freePos.begin() + static_cast<long>(count));

    return nextFreePtrPosArr;
}

Chunks::chunkPos_t
EntityManager::createEntity(const std::string &entityName, C::EntityStatusEnum status)
{
    LOG_DEBUG(BLUE "Creating entity of type " RESET + entityName);
    auto ePool = _getEntityPool(entityName);
    if (std::get<0>(ePool) == nullptr) {
        return {0, 0};
    }
    return _createEntity(ePool, status);
}

Chunks::chunkPos_t EntityManager::_createEntity(
    std::tuple<ECS::E::IEntityPool *, C::entity_pool_id_t> ePool, C::EntityStatusEnum status
)
{
    auto [entityPool, poolId] = ePool;
    auto &freePos = entityPool->getFreePos();
    auto &freePtrPos = _entityPtrPool.getFreePos();

    if (freePos.empty()) {
        entityPool->addChunk();
    }
    if (freePtrPos.empty()) {
        _entityPtrPool.addChunk();
    }
    auto nextFreePos = freePos.front();
    auto nextFreePtrPos = freePtrPos.front();

    auto entity = entityPool->getEntity(Chunks::chunkPos_t(nextFreePos));
    entity->setStatusElem(status);
    entity->setChunkPosElem(Chunks::chunkPos_t(nextFreePtrPos));

    auto entityPtr = _entityPtrPool.getRawEntity(Chunks::chunkPos_t(nextFreePtrPos));
    entityPtr.setStatusElem(C::ENT_ALIVE);
    entityPtr.setChunkPosElem(Chunks::chunkPos_t(nextFreePos));
    entityPtr.setPoolIdElem(poolId);

    freePtrPos.erase(freePtrPos.begin());
    freePos.erase(freePos.begin());

    return {nextFreePtrPos};
}

Chunks::cPosArr_t EntityManager::createEntities(
    const std::string &entityName, size_t count, C::EntityStatusEnum status
)
{
    size_t idx = 0;

    for (auto &entityPool : _entityPools) {
        if (entityPool->getEntityName() == entityName) {
            LOG_DEBUG(
                BLUE "Creating " + std::to_string(count) + " entities of type " RESET + entityName
            );
            return _createEntities(std::make_tuple(entityPool, idx), count, status);
        }
        idx++;
    }
    return {};
}

void EntityManager::destroyEntity(const Chunks::chunkPos_t &cPos)
{
    LOG_DEBUG(RED "Destroying entity" RESET);
    auto entityPtr = _entityPtrPool.getRawEntity(cPos);

    auto poolId = entityPtr.getPoolIdElem();
    auto entCPos = entityPtr.getChunkPosElem();
    auto entity = _entityPools[poolId]->getEntity(entCPos);

    entityPtr.setStatusElem(C::ENT_NONE);
    _entityPtrPool.getFreePos().push_back(cPos);

    entity->setStatusElem(C::ENT_NONE);
    _entityPools[poolId]->getFreePos().push_back(entCPos);
}

void EntityManager::destroyEntities(const Chunks::cPosArr_t &cPosArr)
{
    LOG_DEBUG(RED "Destroying " + std::to_string(cPosArr.size()) + " entities" RESET);

    _entityPtrPool.getEntityStatusPool().setComponentAtIndexes(cPosArr, C::ENT_NONE);
    _entityPtrPool.getFreePos().insert(
        _entityPtrPool.getFreePos().end(), cPosArr.begin(), cPosArr.end()
    );

    Chunks::cPosArr_t deletedcPos;
    deletedcPos.resize(cPosArr.size());
    _entityPtrPool.getChunkPosPool().getComponentsAtIndexes(cPosArr, deletedcPos);

    std::vector<std::tuple<C::entity_pool_id_t>> poolIds;
    poolIds.resize(cPosArr.size());
    _entityPtrPool.getEntityPoolIdPool().getComponentsAtIndexes(cPosArr, poolIds);

    for (size_t i = 0; i < cPosArr.size(); i++) {
        auto [poolId] = poolIds[i];
        auto entCPos = deletedcPos[i];
        auto entity = _entityPools[poolId]->getEntity(entCPos);
        entity->setStatusElem(C::ENT_NONE);
        _entityPools[poolId]->getFreePos().push_back(entCPos);
    }
}

void EntityManager::_destroyEntities(
    const Chunks::cPosArr_t &cPosArr, ECS::E::IEntityPool *entityPool
)
{
    LOG_DEBUG(RED "Destroying " + std::to_string(cPosArr.size()) + " entities" RESET);

    _entityPtrPool.getEntityStatusPool().setComponentAtIndexes(cPosArr, C::ENT_NONE);
    _entityPtrPool.getFreePos().insert(
        _entityPtrPool.getFreePos().end(), cPosArr.begin(), cPosArr.end()
    );

    Chunks::cPosArr_t deletedcPos;
    deletedcPos.resize(cPosArr.size());
    _entityPtrPool.getChunkPosPool().getComponentsAtIndexes(cPosArr, deletedcPos);

    entityPool->getEntityStatusPool().setComponentAtIndexes(deletedcPos, C::ENT_NONE);
    entityPool->getFreePos().insert(
        entityPool->getFreePos().end(), deletedcPos.begin(), deletedcPos.end()
    );
}

void EntityManager::destroyEntities(const Chunks::cPosArr_t &cPosArr, const std::string &entityName)
{
    for (auto &entityPool : _entityPools) {
        if (entityPool->getEntityName() == entityName) {
            _destroyEntities(cPosArr, entityPool);
            return;
        }
    }
}

bool EntityManager::addTime(float time)
{
    _timePassed += time;
    _timeSinceLastFixedUpdate += time;
    _systemTree.deltaTime = time;
    _runSystems();
    if (_timeSinceLastFixedUpdate >= _fixedUpdateTime) {
        _runFixedSystems();
        _timeSinceLastFixedUpdate -= _fixedUpdateTime;
        return true;
    }
    return false;
}

void EntityManager::_runSystems() { _systemTree.runTree(); }

void EntityManager::_runFixedSystems() { _fixedSystemTree.runTree(); }

std::tuple<ECS::E::IEntityPool *, C::entity_pool_id_t>
EntityManager::_getEntityPool(const std::string &entityName)
{
    size_t idx = 0;
    for (auto &entityPool : _entityPools) {
        if (entityPool->getEntityName() == entityName) {
            return {entityPool, idx};
        }
        idx++;
    }
    return {nullptr, 0};
}

} // namespace ECS