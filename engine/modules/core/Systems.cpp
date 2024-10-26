#include "Systems.hpp"

namespace ECS::S {

void IncrementTimerSystem::_statusOperate(typename C::TimerPool::Types &ctimer)
{
    auto [time] = ctimer;
    time += _deltaTime;
}

}