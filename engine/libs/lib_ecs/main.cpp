/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Components/PureComponentPools.hpp"
#include "Demo/ExamplePoint.hpp"
#include "Demo/Systems.hpp"
#include "EntityManager.hpp"
#include "Systems/Query.hpp"

using namespace ECS;

int main()
{
  // INITIALIZATION
  EntityManager _eM = EntityManager();

  S::InitSystem initSystem = S::InitSystem();
  S::MoveUpSystem moveUpSystem = S::MoveUpSystem();
  S::PrintSystem printSystem = S::PrintSystem();
  S::DualExampleSystem dualExampleSystem = S::DualExampleSystem();

  E::ExamplePointPool pointsPool = E::ExamplePointPool();
  // pointsPool.addChunk();


  // Register the systems
  _eM.registerSystem(initSystem, S::ROOTSYSGROUP);
  _eM.registerSystem(moveUpSystem, S::ROOTSYSGROUP);
  _eM.registerSystem(printSystem, S::ROOTSYSGROUP);
  _eM.registerSystem(dualExampleSystem, S::ROOTSYSGROUP);
  // Register the entity pools
  _eM.registerEntityPool(&pointsPool);

  // Create entities with a status
  auto entities = _eM.createEntities("ExamplePoint", 5, C::ENT_NEEDS_INIT);
  _eM.runSystems();

  using SimpleQuery = S::Query<C::EntityStatusPool, C::ChunkPosPool>;

  // Run a query
  // There is a version without the ent manager, but you will need to initialize the query yourself
  SimpleQuery().map(_eM, [](auto &cStatus, auto &cChunkPos) {
    auto [status] = cStatus;
    auto [cIdx, eIdx] = cChunkPos;
    std::cout << "Entity Status: " << C::EntityStatusEnum(status) << " ChunkPos: [" << cIdx << ", " << eIdx << "]\n";
  });

  // Run a 'cross' query, the template of cross is the second query
  // You can also use cross with lvalues, without the ent manager. (both will need to be initialized):
  // firstQuery.cross(secondQuery, ...), no template args needed, will be inferred from secondQuery
  SimpleQuery().cross<C::ChunkPosPool>(_eM, [](auto &cStatusA, auto &cChunkPosA, auto &cChunkPosB) {
    auto [status] = cStatusA;
    auto [cIdx2, eIdx2] = cChunkPosB;
    // if not the same ent, print an return
    if (cChunkPosA != cChunkPosB) {
      std::cout << "ents are not the same\n";
      return;
    }
    std::cout << "Entity Status: " << C::EntityStatusEnum(status) << " ChunkPos: [" << cIdx2 << ", " << eIdx2 << "]\n";
  });


  // Create more entities
  auto entities2 = _eM.createEntities("ExamplePoint", 5, C::ENT_NEEDS_INIT);
  _eM.runSystems();

  // Destroy entities
  _eM.destroyEntities(entities);
  _eM.runSystems();
  _eM.destroyEntities(entities2);
  _eM.runSystems();

  // Create more entities
  auto lastEntities = _eM.createEntities("ExamplePoint", 11, C::ENT_NEEDS_INIT);
  _eM.runSystems();

  // Call a system yourself
  printSystem.run();

  // Destroy some entities
  _eM.destroyEntity(lastEntities[0]);
  _eM.destroyEntity(lastEntities[1]);
  _eM.destroyEntity(lastEntities[2]);
  _eM.runSystems();

  // Create more entities
  auto trueLastEntities = _eM.createEntities("ExamplePoint", 10, C::ENT_NEEDS_INIT);
  _eM.runSystems();

  // No memory leaks
  std::cout << "\n" << std::flush;

  return 0;
}