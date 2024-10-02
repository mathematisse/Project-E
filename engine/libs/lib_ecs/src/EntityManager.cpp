/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#include "EntityManager.hpp"
#include "Chunks/ChunkPos.hpp"
#include "Components/PureComponentPools.hpp"
#include "Entities/IEntity.hpp"
#include "Entities/PureEntities.hpp"
#include "Systems/SystemTree.hpp"
#include <list>
#include <iostream>

ECS::EntityManager::EntityManager()
{
    _entityPtrPool.addChunk();
}

ECS::EntityManager::~EntityManager() = default;

bool ECS::EntityManager::registerSystemGroup(int group, int neighbourGroup, bool addBefore, bool addInside)
{
    return _systemTree.addSystemGroup(group, neighbourGroup, addBefore, addInside);
}

bool ECS::EntityManager::registerSystem(Systems::ISystem &system, int group, bool atStart)
{
    for (auto &entityPool : _entityPools) {
        system.tryAddEntityPool(entityPool);
    }
    return _systemTree.addSystem(&system, group, atStart);
}

bool ECS::EntityManager::registerEntityPool(Entities::IEntityPool *entityPool)
{
    _systemTree.registerEntityPool(entityPool);
    _entityPools.push_back(entityPool);
    return true;
}

std::unique_ptr<ECS::Entities::IEntity>ECS::EntityManager::getEntity(const ECS::Entities::EntityPtr &entityPtr)
{
    return (*(_entityPools[entityPtr.getPoolId()])).getEntity(entityPtr.getChunkPos());
}

std::vector<std::unique_ptr<ECS::Entities::EntityPtr>> ECS::EntityManager::_createEntities(ECS::Entities::IEntityPool *entityPool, size_t count, size_t poolId)
{
    std::list<Chunks::ChunkPos> &freePos = entityPool->getFreePos();
    std::list<Chunks::ChunkPos> &freePtrPos = _entityPtrPool.getFreePos();

    std::vector<std::unique_ptr<ECS::Entities::EntityPtr>> entities;
    // ECS::Entities::IEntity* entity;

    for (size_t i = 0; i < count; i++) {
        std::cout << "creating " << i << " of " << count << " entities\n";
        if (freePos.empty()) {
            entityPool->addChunk();
        }
        if (freePtrPos.empty()) {
            _entityPtrPool.addChunk();
        }
        auto nextFreePos = freePos.front();
        auto nextFreePtrPos = freePtrPos.front();

        std::cout << "nextFreePos: " << nextFreePos.chunkIndex << " " << nextFreePos.elemIndex<< "\n";
        std::cout << "nextFreePtrPos: " << nextFreePtrPos.chunkIndex << " " << nextFreePtrPos.elemIndex<< "\n";

        auto entity = entityPool->getEntity(nextFreePos);
        entity->setStatus(Components::ENT_NEEDS_INIT);
        entity->setChunkPos(nextFreePtrPos);

        auto *entityPtr = _entityPtrPool.getRawEntity(nextFreePtrPos);
        entityPtr->setStatus(Components::ENT_ALIVE);
        entityPtr->setChunkPos(nextFreePos);
        entityPtr->setPoolId(poolId);

        entities.push_back(std::unique_ptr<ECS::Entities::EntityPtr>(entityPtr));

        freePtrPos.pop_front();
        freePos.pop_front();
    }
    return entities;
}

std::unique_ptr<ECS::Entities::EntityPtr>ECS::EntityManager::createEntity(const std::string &entityName)
{
    std::vector<std::unique_ptr<ECS::Entities::EntityPtr>> entities = createEntities(entityName, 1);
    return entities.empty() ? nullptr : std::move(entities[0]);
}

std::vector<std::unique_ptr<ECS::Entities::EntityPtr>> ECS::EntityManager::createEntities(const std::string &entityName, size_t count)
{
    size_t idx = 0;

    for (auto &entityPool : _entityPools) {
        if (entityPool->getEntityName() == entityName) {
            return _createEntities(entityPool, count, idx);
        }
        idx++;
    }
    return {};
}

void ECS::EntityManager::destroyEntity(std::unique_ptr<ECS::Entities::EntityPtr> &entity)
{
    entity->setStatus(Components::ENT_NONE);
    _entityPtrPool.getFreePos().push_back(entity->getChunkPos());
    (*(_entityPools[entity->getPoolId()])).getEntity(entity->getChunkPos())->setStatus(Components::ENT_NONE);
    (*(_entityPools[entity->getPoolId()])).getEntity(entity->getChunkPos())->setChunkPos({0, 0});
    (*(_entityPools[entity->getPoolId()])).getFreePos().push_back(entity->getChunkPos());
    entity->setChunkPos({0, 0});
}

void ECS::EntityManager::destroyEntities(std::vector<std::unique_ptr<ECS::Entities::EntityPtr>> &entities)
{
    for (auto &entity : entities) {
        destroyEntity(entity);
    }
}

void ECS::EntityManager::runSystems()
{
    _systemTree.runTree();
}