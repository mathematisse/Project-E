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
    entityManager.registerSystem(dynamic_cast<ECS::Systems::ISystem&>(moveUpSystem), ECS::Systems::ROOTSYSGROUP);
    entityManager.registerEntityPool(&pointGoingUpEntityPool);

    // Create entities
    auto entities = entityManager.createEntities("pointGoingUp", 10);

    // Run the system
    entityManager.runSystems();

    std::cout << "You've successfully compiled runned, and smashed clang-tidy against a sharpy rock !\n";

    return 0;
}