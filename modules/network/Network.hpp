/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Render ecs module
*/

#pragma once

#include "IModule.hpp"
#include "Components.hpp" // IWYU pragma: keep
#include "Systems.hpp"
#include "lib_ecs/Systems/ExecutionTypes.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"

#define NETWORK_SYS_GROUP "NETWORK"

namespace engine::module {

class Network : public IModule {

    ECS::S::SystemTreeNode networkNode;

public:
    Network():
        networkNode(ECS::S::SERIAL_NODE_EXECUTION, NETWORK_SYS_GROUP, {})
    {
    }

    void load(ECS::EntityManager &entityManager) override
    {
        entityManager.registerSystemNode(networkNode, ROOT_SYS_GROUP);
    }
};

}
