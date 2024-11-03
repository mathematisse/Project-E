#include "Systems.hpp"
#include "lib_ecs/Systems/ASystem.hpp"
#include "lib_ecs/Systems/ExecutionTypes.hpp"

namespace ECS::S {

IncrementTimerSystem::IncrementTimerSystem():
    ASystem(PARALLEL_SYSTEM_EXECUTION)
{
}

void IncrementTimerSystem::_innerOperate(typename C::Timer::Pool::Types &ctimer)
{
    auto [time] = ctimer;
    time += _deltaTime;
}

}