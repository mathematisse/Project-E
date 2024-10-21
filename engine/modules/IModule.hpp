#pragma once

#include "lib_ecs/EntityManager.hpp"

namespace engine {
namespace module {

class IModule {
public:
    virtual ~IModule() = default;
    virtual void load(ECS::EntityManager &entityManager) = 0;
};

};
}