#pragma once

#include "IModule.hpp"
#include "AComponentPools.hpp"
#include "ASystem.hpp"

namespace engine {
class SpatialModule : public IModule {
public:
    SpatialModule() = default;
    ~SpatialModule() = default;

    void init(ECS::EntityManager &entityManager) override;
};

///////////////////////////////////////////////////////////////////////////

/// Components

class PositionComponentPool : public ECS::Components::AComponentPool2<int> {
public:
    PositionComponentPool();
};

/// Systems

class MoveSystem : public ECS::Systems::ASystem {
public:
    MoveSystem(unsigned int velocity);
    void run() override;

private:
    unsigned int _velocity;
};

}
