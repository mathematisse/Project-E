#pragma once

#include "engine/modules/IModule.hpp"
#include "engine/modules/dummy/Components.hpp"
#include "engine/modules/dummy/Systems.hpp"

namespace engine {
namespace module {

class DummyModule : public IModule {
    ECS::S::RandomizeNumberSystem randomizeNumberSystem;
    ECS::S::ShowNumberSystem showNumberSystem;

public:
    void load(ECS::EntityManager &entityManager) override
    {
        entityManager.registerSystem(randomizeNumberSystem, ECS::S::ROOTSYSGROUP, false);
        entityManager.registerSystem(showNumberSystem, ECS::S::ROOTSYSGROUP, false);
    }
};

};
}