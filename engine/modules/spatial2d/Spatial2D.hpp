#pragma once

#include "IModule.hpp"
#include "Components.hpp" // IWYU pragma: keep
#include "Systems.hpp"

namespace engine::module {

class Spatial2D : public IModule {
    ECS::S::ApplyVelocitySystem applyVelocitySystem;
    ECS::S::ApplyAccelerationSystem applyAccelerationSystem;

public:
    void load(ECS::EntityManager &entityManager) override
    {
        entityManager.registerSystem(applyVelocitySystem, ECS::S::ROOTSYSGROUP, false);
        entityManager.registerSystem(applyAccelerationSystem, ECS::S::ROOTSYSGROUP, false);
    }
};

}
