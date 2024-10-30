#include "Systems.hpp"

namespace ECS::S {

void IncrementTimerSystem::_innerOperate(typename C::Timer::Pool::Types &ctimer)
{
    auto [time] = ctimer;
    time += _deltaTime;
}

}