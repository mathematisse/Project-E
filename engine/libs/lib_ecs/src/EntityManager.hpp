/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#pragma once

#include "Chunks/ChunkPos.hpp"
#include "Entities/IEntityPool.hpp"
#include "Entities/IEntityRef.hpp"
#include "Entities/PureEntities.hpp"
#include "Entities/PureEntityPools.hpp"
#include "Systems/IQuery.hpp"
#include "Systems/SystemTree.hpp"
#include <memory>
#include <vector>

namespace ECS {
class EntityManager
{
public:
  bool registerSystemGroup(int group, int neighbourGroup, bool addBefore = false, bool addInside = false);
  bool registerSystem(S::ISystem &system, int group, bool atStart = false);
  bool registerEntityPool(ECS::E::IEntityPool *entityPool);
  S::IQuery &initializeQuery(S::IQuery &query);
  std::unique_ptr<ECS::E::IEntityRef> getEntity(const ECS::E::EntityPtrRef &entityPtr);
  std::unique_ptr<ECS::E::IEntityRef> getEntity(const Chunks::ChunkPos &cPos);
  Chunks::ChunkPos createEntity(const std::string &entityName, C::EntityStatusEnum status = C::ENT_ALIVE);
  std::vector<Chunks::ChunkPos>
    createEntities(const std::string &entityName, size_t count = 0, C::EntityStatusEnum status = C::ENT_ALIVE);
  void destroyEntity(const Chunks::ChunkPos &cPos);
  void destroyEntities(const std::vector<Chunks::ChunkPos> &cPosArr);

  void runSystems();

private:
  std::vector<Chunks::ChunkPos>
    _createEntities(ECS::E::IEntityPool *entityPool, size_t count, size_t poolId, C::EntityStatusEnum status);

  ECS::E::EntityPtrPool _entityPtrPool;
  std::vector<ECS::E::IEntityPool *> _entityPools;
  S::SystemTree _systemTree;
};
}// namespace ECS
