/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#include "EntityManager.hpp"
#include "Chunks/ChunkPos.hpp"
#include "Components/PureComponentPools.hpp"
#include "Entities/PureEntities.hpp"
#include "Systems/SystemTree.hpp"
#include <list>
#include <iostream>

namespace ECS {

EntityManager::EntityManager()
{
    _entityPtrPool.addChunk();
}

EntityManager::~EntityManager() = default;

bool EntityManager::registerSystemGroup(int group, int neighbourGroup, bool addBefore, bool addInside)
{
    return _systemTree.addSystemGroup(group, neighbourGroup, addBefore, addInside);
}

bool EntityManager::registerSystem(Systems::ISystem &system, int group, bool atStart)
{
    for (auto &entityPool : _entityPools) {
        system.tryAddEntityPool(entityPool);
    }
    return _systemTree.addSystem(&system, group, atStart);
}

bool EntityManager::registerEntityPool(Entities::IEntityPool *entityPool)
{
    _systemTree.registerEntityPool(entityPool);
    _entityPools.push_back(entityPool);
    return true;
}

std::unique_ptr<Entities::IEntityRef>EntityManager::getEntity(const Entities::EntityPtrRef &entityPtr)
{
    return (*(_entityPools[entityPtr.getPoolId()])).getEntity(entityPtr.getChunkPos());
}

std::unique_ptr<Entities::IEntityRef>EntityManager::getEntity(const Chunks::ChunkPos &cPos)
{
    auto entityPtr = _entityPtrPool.getRawEntity(cPos);
    return (*(_entityPools[entityPtr->getPoolId()])).getEntity(entityPtr->getChunkPos());
}

std::vector<Chunks::ChunkPos> EntityManager::_createEntities(Entities::IEntityPool *entityPool, size_t count, size_t poolId, Components::EntityStatusEnum status)
{
    std::list<Chunks::ChunkPos> &freePos = entityPool->getFreePos();
    std::list<Chunks::ChunkPos> &freePtrPos = _entityPtrPool.getFreePos();

    std::vector<Chunks::ChunkPos> cPosArr;
    // Entities::IEntity* entity;

    for (size_t i = 0; i < count; i++) {
        if (freePos.empty()) {
            entityPool->addChunk();
        }
        if (freePtrPos.empty()) {
            _entityPtrPool.addChunk();
        }
        auto nextFreePos = freePos.front();
        auto nextFreePtrPos = freePtrPos.front();

        auto entity = entityPool->getEntity(nextFreePos);
        entity->setStatus(status);
        entity->setChunkPos(nextFreePtrPos);

        auto entityPtr = _entityPtrPool.getRawEntity(nextFreePtrPos);
        entityPtr->setStatus(Components::ENT_ALIVE);
        entityPtr->setChunkPos(nextFreePos);
        entityPtr->setPoolId(poolId);

        cPosArr.push_back(nextFreePos);

        freePtrPos.pop_front();
        freePos.pop_front();
    }
    return cPosArr;
}

Chunks::ChunkPos EntityManager::createEntity(const std::string &entityName, Components::EntityStatusEnum status)
{
    auto cPosArr = createEntities(entityName, 1, status);
    return cPosArr.empty() ? Chunks::ChunkPos{0, 0} : cPosArr[0];
}

std::vector<Chunks::ChunkPos> EntityManager::createEntities(const std::string &entityName, size_t count, Components::EntityStatusEnum status)
{
    size_t idx = 0;

    for (auto &entityPool : _entityPools) {
        if (entityPool->getEntityName() == entityName) {
            std::cout << "Creating " << count << " entities of type " << entityName << "\n";
            return _createEntities(entityPool, count, idx, status);
        }
        idx++;
    }
    return {};
}

void EntityManager::destroyEntity(const Chunks::ChunkPos &cPos)
{
    auto entityPtr = std::unique_ptr<Entities::EntityPtrRef>(_entityPtrPool.getRawEntity(cPos));

    auto poolId = entityPtr->getPoolId();
    auto entCPos = entityPtr->getChunkPos();
    auto entity = _entityPools[poolId]->getEntity(entCPos);

    entityPtr->setStatus(Components::ENT_NONE);
    entityPtr->setChunkPos({0, 0});
    _entityPtrPool.getFreePos().push_back(cPos);

    entity->setStatus(Components::ENT_NONE);
    entity->setChunkPos({0, 0});
    _entityPools[poolId]->getFreePos().push_back(entCPos);
}

void EntityManager::destroyEntities(const std::vector<Chunks::ChunkPos> &cPosArr)
{
    for (const auto &cPos : cPosArr) {
        destroyEntity(cPos);
    }
}

void EntityManager::runSystems()
{
    _systemTree.runTree();
}

}