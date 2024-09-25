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
#include "Systems/SystemTree.hpp"
#include <list>

ECS::EntityManager::EntityManager()
{
    _entityPtrPool.addChunk();
}

ECS::EntityManager::~EntityManager()
{
}

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

ECS::Entities::IEntity*ECS::EntityManager::operator[](const ECS::Entities::EntityPtr &entityPtr)
{
    return (*(_entityPools[entityPtr.getPoolId()]))[entityPtr.getChunkPos()];
}

std::vector<ECS::Entities::EntityPtr *> ECS::EntityManager::_createEntities(ECS::Entities::IEntityPool *entityPool, size_t count)
{
    std::list<Chunks::ChunkPos> &freePos = entityPool->getFreePos();
    std::list<Chunks::ChunkPos> &freePtrPos = _entityPtrPool.getFreePos();

    std::vector<ECS::Entities::EntityPtr *> entities;
    // ECS::Entities::IEntity* entity;

    std::cout << "WOW freePos.size() = " << freePos.size() << '\n';
    for (size_t i = 0; i < count; i++) {
        std::cout << " NEW INSERT freePos.size() = " << freePos.size() << '\n';
        if (freePos.empty()) {
            std::cout << "addChunk\n";
            entityPool->addChunk();
        }
        if (freePtrPos.empty()) {
            std::cout << "addChunk\n";
            _entityPtrPool.addChunk();
        }
        entities.push_back(dynamic_cast<ECS::Entities::EntityPtr *>(_entityPtrPool[freePtrPos.front()].get()));
        std::cout << " WILL NOW GET THE ENTITY freePos.front() = " << freePos.front().chunkIndex << " " << freePos.front().elemIndex << '\n';
        // entity = (*entityPool)[freePos.front()];
        (*entityPool)[freePos.front()]->setChunkPos(freePtrPos.front());
        (*entityPool)[freePos.front()]->setStatus(Components::ENT_NEEDS_INIT);
        entities.back()->setChunkPos(freePos.front());
        entities.back()->setStatus(Components::ENT_ALIVE);
        freePtrPos.pop_front();
        freePos.pop_front();
    }
    return entities;
}

ECS::Entities::EntityPtr *ECS::EntityManager::createEntity(const std::string &entityName)
{
    std::vector<ECS::Entities::EntityPtr *> entities = createEntities(entityName, 1);
    return entities.empty() ? nullptr : entities[0];
}

std::vector<ECS::Entities::EntityPtr *> ECS::EntityManager::createEntities(const std::string &entityName, size_t count)
{
    for (auto &entityPool : _entityPools) {
        if (entityPool->getEntityName() == entityName) {
            return _createEntities(entityPool, count);
        }
    }
    return {};
}

void ECS::EntityManager::destroyEntity(ECS::Entities::EntityPtr *entity)
{
    entity->setStatus(Components::ENT_NONE);
    _entityPtrPool.getFreePos().push_back(entity->getChunkPos());
    (*(_entityPools[entity->getPoolId()]))[entity->getChunkPos()]->setStatus(Components::ENT_NONE);
    (*(_entityPools[entity->getPoolId()]))[entity->getChunkPos()]->setChunkPos({0, 0});
    (*(_entityPools[entity->getPoolId()])).getFreePos().push_back(entity->getChunkPos());
    entity->setChunkPos({0, 0});
}

void ECS::EntityManager::destroyEntities(const std::vector<ECS::Entities::EntityPtr *> &entities)
{
    for (const auto &entity : entities) {
        destroyEntity(entity);
    }
}

void ECS::EntityManager::runSystems()
{
    _systemTree.runTree();
}