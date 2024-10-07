#include "TestModule.hpp"

void engine::SpatialModule::init(ECS::EntityManager &entityManager) { }

engine::PositionComponentPool::PositionComponentPool():
    ECS::Components::AComponentPool<uint64_t>("position")
{
}

engine::MoveSystem::MoveSystem(unsigned int velocity):
    ECS::Systems::ASystem({"position"}, false),
    _velocity(velocity)
{
}

void engine::MoveSystem::run() { }
