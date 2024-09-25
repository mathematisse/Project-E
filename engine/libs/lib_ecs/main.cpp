/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "PointGoingUp.hpp"
#include "EntityManager.hpp"

int main() {
    ECS::EntityManager entityManager = ECS::EntityManager();


    ECS::MoveUpSystem moveUpSystem = ECS::MoveUpSystem();
    ECS::PointGoingUpEntityPool pointGoingUpEntityPool = ECS::PointGoingUpEntityPool();

    pointGoingUpEntityPool.addChunk();

    // Register the system with the entity manager
    entityManager.registerSystem(dynamic_cast<ECS::Systems::ISystem&>(moveUpSystem), 0);
    entityManager.registerEntityPool(&pointGoingUpEntityPool);

    // Create entities
    auto entities = entityManager.createEntities("pointGoingUp", 10);

    // Run the system
    entityManager.runSystems();

    return 0;
}