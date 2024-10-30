#pragma once

#include "IModule.hpp"
#include "Components.hpp" // IWYU pragma: keep

namespace engine::module {

class Game : public IModule {
public:
    void load(ECS::EntityManager &entityManager) override { }
};

}