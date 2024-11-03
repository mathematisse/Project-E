#pragma once

#include "IModule.hpp"
#include "Components.hpp" // IWYU pragma: keep
#include "Systems.hpp"
#include "lib_log/log.hpp"

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
        LOG_INFO("Loading Spatial2D module");
        if (!entityManager.registerFixedSystemNode(spatial2DNode, FIXED_ROOT_SYS_GROUP)) {
            LOG_ERROR("Failed to register Spatial2D system node");
        }
    }
};

}
