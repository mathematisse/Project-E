#pragma once

#include "IModule.hpp"
#include "Components.hpp" // IWYU pragma: keep
#include "Systems.hpp"

namespace engine::module {

class Core : public IModule {
    ECS::S::IncrementTimerSystem incrementTimerSystem;

public:
    void load(ECS::EntityManager &entityManager) override
    {
        entityManager.registerSystem(incrementTimerSystem, ECS::S::ROOTSYSGROUP, false);
    }
};

}