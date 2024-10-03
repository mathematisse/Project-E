/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "PointGoingUp.hpp"
#include "EntityManager.hpp"
#include "Systems/ISystem.hpp"

int main() {
    ECS::EntityManager entityManager = ECS::EntityManager();


    ECS::MoveUpSystem moveUpSystem = ECS::MoveUpSystem();
    ECS::PointGoingUpEntityPool pointGoingUpEntityPool = ECS::PointGoingUpEntityPool();

    pointGoingUpEntityPool.addChunk();

    // Register the system with the entity manager
    entityManager.registerSystem(*static_cast<ECS::Systems::ISystem *>(&moveUpSystem), ECS::Systems::ROOTSYSGROUP);
    entityManager.registerEntityPool(&pointGoingUpEntityPool);

    // Create entities
    auto entities = entityManager.createEntities("pointGoingUp", 5);

    // Run the system
    entityManager.runSystems();

    auto entities2 = entityManager.createEntities("pointGoingUp", 5);

    // Run the system
    entityManager.runSystems();

    entityManager.destroyEntities(entities);

    // Run the system
    entityManager.runSystems();

    entityManager.destroyEntities(entities2);

    // Run the system
    entityManager.runSystems();

    auto lastEntities = entityManager.createEntities("pointGoingUp", 11);

    // Run the system
    entityManager.runSystems();

    entityManager.destroyEntity(lastEntities[0]);
    entityManager.destroyEntity(lastEntities[1]);
    entityManager.destroyEntity(lastEntities[2]);

    // Run the system
    entityManager.runSystems();

    auto trueLastEntities = entityManager.createEntities("pointGoingUp", 10);


    // Run the system
    entityManager.runSystems();


    return 0;
}