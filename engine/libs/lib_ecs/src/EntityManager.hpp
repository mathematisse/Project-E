/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#pragma once

#include "Chunks/ChunkPos.hpp"
#include "Components/PureComponentPools.hpp"
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
  std::unique_ptr<ECS::E::IEntityRef> getEntity(const Chunks::chunkPos_t &cPos);
  Chunks::chunkPos_t createEntity(const std::string &entityName, C::EntityStatusEnum status = C::ENT_ALIVE);
  Chunks::cPosArr_t
    createEntities(const std::string &entityName, size_t count = 0, C::EntityStatusEnum status = C::ENT_ALIVE);
  void destroyEntity(const Chunks::chunkPos_t &cPos);
  void destroyEntities(const Chunks::cPosArr_t &cPosArr);
  void destroyEntities(const Chunks::cPosArr_t &cPosArr, const std::string &entityName);

  void runSystems();

private:
  Chunks::cPosArr_t
    _createEntities(std::tuple<ECS::E::IEntityPool *, C::entity_pool_id_t>, size_t count, C::EntityStatusEnum status);
  Chunks::chunkPos_t _createEntity(std::tuple<ECS::E::IEntityPool *, C::entity_pool_id_t>, C::EntityStatusEnum status);
  std::tuple<ECS::E::IEntityPool *, C::entity_pool_id_t> _getEntityPool(const std::string &entityName);
  void _destroyEntities(const Chunks::cPosArr_t &cPosArr, ECS::E::IEntityPool *entityPool);

  ECS::E::EntityPtrPool _entityPtrPool;
  std::vector<ECS::E::IEntityPool *> _entityPools;
  S::SystemTree _systemTree;
};
}// namespace ECS
