/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Demo/ExamplePoint.hpp"
#include "Demo/Systems.hpp"
#include "EntityManager.hpp"
#include "Systems/ISystem.hpp"

int main() {
    // INITIALIZATION
    ECS::EntityManager entityManager = ECS::EntityManager();

    ECS::InitSystem initSystem = ECS::InitSystem();
    ECS::MoveUpSystem moveUpSystem = ECS::MoveUpSystem();
    ECS::PrintSystem printSystem = ECS::PrintSystem();
    ECS::DualExampleSystem dualExampleSystem = ECS::DualExampleSystem();

    ECS::Entities::ExamplePointPool pointsPool = ECS::Entities::ExamplePointPool();
    pointsPool.addChunk();

    // Register the system with the entity manager
    entityManager.registerSystem(*static_cast<ECS::Systems::ISystem *>(&initSystem), ECS::Systems::ROOTSYSGROUP);
    entityManager.registerSystem(*static_cast<ECS::Systems::ISystem *>(&moveUpSystem), ECS::Systems::ROOTSYSGROUP);
    entityManager.registerSystem(*static_cast<ECS::Systems::ISystem *>(&printSystem), ECS::Systems::ROOTSYSGROUP);
    entityManager.registerSystem(*static_cast<ECS::Systems::ISystem *>(&dualExampleSystem), ECS::Systems::ROOTSYSGROUP);
    entityManager.registerEntityPool(&pointsPool);

    // Create entities
    auto entities = entityManager.createEntities("ExamplePoint", 5, ECS::Components::ENT_NEEDS_INIT);
    entityManager.runSystems();

    auto entities2 = entityManager.createEntities("ExamplePoint", 5, ECS::Components::ENT_NEEDS_INIT);
    entityManager.runSystems();

    // Destroy entities
    entityManager.destroyEntities(entities);
    entityManager.runSystems();
    entityManager.destroyEntities(entities2);
    entityManager.runSystems();

    // Create more entities
    auto lastEntities = entityManager.createEntities("ExamplePoint", 11, ECS::Components::ENT_NEEDS_INIT);
    entityManager.runSystems();
    
    // Call a system yourself
    printSystem.run();

    // Destroy some entities
    entityManager.destroyEntity(lastEntities[0]);
    entityManager.destroyEntity(lastEntities[1]);
    entityManager.destroyEntity(lastEntities[2]);
    entityManager.runSystems();

    // Create more entities
    auto trueLastEntities = entityManager.createEntities("ExamplePoint", 10, ECS::Components::ENT_NEEDS_INIT);
    entityManager.runSystems();

    // No memory leaks
    std::cout << "\n" << std::flush;

    return 0;
}