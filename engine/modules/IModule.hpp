#pragma once

#include "lib_ecs/EntityManager.hpp"

namespace engine {
namespace module {

class IModule {
public:
    virtual ~IModule() = default;
    IModule(const IModule &) = default;
    IModule &operator=(const IModule &) = default;
    IModule(IModule &&) = default;
    IModule &operator=(IModule &&) = default;

    virtual void load(ECS::EntityManager &entityManager) = 0;
};

};
}