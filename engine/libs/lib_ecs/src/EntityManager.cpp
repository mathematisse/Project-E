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
#include <iostream>
#include <list>

#define BLUE "\033[34m"
#define RED "\033[31m"
#define RESET "\033[0m"

namespace ECS {

bool EntityManager::registerSystemGroup(int group, int neighbourGroup, bool addBefore, bool addInside)
{
  return _systemTree.addSystemGroup(group, neighbourGroup, addBefore, addInside);
}

bool EntityManager::registerSystem(S::ISystem &system, int group, bool atStart)
{
  for (auto &entityPool : _entityPools) { system.tryAddEntityPool(entityPool); }
  return _systemTree.addSystem(&system, group, atStart);
}

bool EntityManager::registerEntityPool(E::IEntityPool *entityPool)
{
  _systemTree.registerEntityPool(entityPool);
  _entityPools.push_back(entityPool);
  return true;
}

S::IQuery &EntityManager::initializeQuery(S::IQuery &query)
{
  std::cout << "\n" BLUE "Initializing Query" RESET "\n";
  for (auto &entityPool : _entityPools) { query.tryAddEntityPool(entityPool); }
  return query;
}

std::unique_ptr<E::IEntityRef> EntityManager::getEntity(const E::EntityPtrRef &entityPtr)
{
  return (*(_entityPools[entityPtr.getPoolId()])).getEntity(entityPtr.getChunkPos());
}

std::unique_ptr<E::IEntityRef> EntityManager::getEntity(const Chunks::ChunkPos &cPos)
{
  auto entityPtr = _entityPtrPool.getRawEntity(cPos);
  return (*(_entityPools[entityPtr->getPoolId()])).getEntity(entityPtr->getChunkPos());
}

std::vector<Chunks::ChunkPos>
  EntityManager::_createEntities(E::IEntityPool *entityPool, size_t count, size_t poolId, C::EntityStatusEnum status)
{
  std::vector<Chunks::ChunkPos> &freePos = entityPool->getFreePos();
  std::vector<Chunks::ChunkPos> &freePtrPos = _entityPtrPool.getFreePos();

  std::vector<Chunks::ChunkPos> cPosArr;
  // E::IEntity* entity;

  for (size_t i = 0; i < count; i++) {
    if (freePos.empty()) { entityPool->addChunk(); }
    if (freePtrPos.empty()) { _entityPtrPool.addChunk(); }
    auto nextFreePos = freePos.front();
    auto nextFreePtrPos = freePtrPos.front();

    auto entity = entityPool->getEntity(nextFreePos);
    entity->setStatus(status);
    entity->setChunkPos(nextFreePtrPos);

    auto entityPtr = _entityPtrPool.getRawEntity(nextFreePtrPos);
    entityPtr->setStatus(C::ENT_ALIVE);
    entityPtr->setChunkPos(nextFreePos);
    entityPtr->setPoolId(poolId);

    cPosArr.push_back(nextFreePtrPos);

    freePtrPos.erase(freePtrPos.begin());
    freePos.erase(freePos.begin());
  }
  return cPosArr;
}

Chunks::ChunkPos EntityManager::createEntity(const std::string &entityName, C::EntityStatusEnum status)
{
  auto cPosArr = createEntities(entityName, 1, status);
  return cPosArr.empty() ? Chunks::ChunkPos{ 0, 0 } : cPosArr[0];
}

std::vector<Chunks::ChunkPos>
  EntityManager::createEntities(const std::string &entityName, size_t count, C::EntityStatusEnum status)
{
  size_t idx = 0;

  for (auto &entityPool : _entityPools) {
    if (entityPool->getEntityName() == entityName) {
      std::cout << "\n" BLUE "Creating " << count << " entities of type " RESET << entityName << "\n";
      return _createEntities(entityPool, count, idx, status);
    }
    idx++;
  }
  return {};
}

void EntityManager::destroyEntity(const Chunks::ChunkPos &cPos)
{
  std::cout << "\n" RED "Destroying entity" RESET "\n";
  auto entityPtr = std::unique_ptr<E::EntityPtrRef>(_entityPtrPool.getRawEntity(cPos));

  auto poolId = entityPtr->getPoolId();
  auto entCPos = entityPtr->getChunkPos();
  auto entity = _entityPools[poolId]->getEntity(entCPos);

  entityPtr->setStatus(C::ENT_NONE);
  _entityPtrPool.getFreePos().push_back(cPos);

  entity->setStatus(C::ENT_NONE);
  _entityPools[poolId]->getFreePos().push_back(entCPos);
}

void EntityManager::destroyEntities(const std::vector<Chunks::ChunkPos> &cPosArr)
{
  std::cout << "\n" RED "Destroying " << cPosArr.size() << " entities" RESET "\n";
  for (const auto &cPos : cPosArr) { destroyEntity(cPos); }
}

void EntityManager::runSystems() { _systemTree.runTree(); }

}// namespace ECS