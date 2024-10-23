#pragma once

#include "IModule.hpp"
#include "Components.hpp" // IWYU pragma: keep
#include "Systems.hpp"

#define SPATIAL2D_SYS_GROUP "SPATIAL2D"

namespace engine::module {

class Spatial2D : public IModule {

    ECS::S::ApplyAccelerationSystem applyAccelerationSystem;
    ECS::S::ApplyVelocitySystem applyVelocitySystem;

    ECS::S::SystemTreeNode spatial2DNode;

public:

    Spatial2D():
        spatial2DNode(SPATIAL2D_SYS_GROUP, {&applyAccelerationSystem, &applyVelocitySystem})
    {
    }

    void load(ECS::EntityManager &entityManager) override
    {
        entityManager.registerFixedSystemNode(spatial2DNode, ROOT_SYS_GROUP);
    }
};

}
