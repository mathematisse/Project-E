#pragma once

#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "Components.hpp"

namespace ECS::S {

class IncrementTimerSystem : public S::AStatusMonoSystem<C::TimerPool> {
public:
    ~IncrementTimerSystem() override = default;

    IncrementTimerSystem(const IncrementTimerSystem &other) = default;
    IncrementTimerSystem(IncrementTimerSystem &&other) = default;
    IncrementTimerSystem &operator=(const IncrementTimerSystem &other) = default;
    IncrementTimerSystem &operator=(IncrementTimerSystem &&other) = default;

    float deltaTime = 0.1F;

protected:
    void _statusOperate(typename C::TimerPool::Types &ctimer) override;
};

}