#pragma once

#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "Components.hpp"

namespace ECS::S {

class IncrementTimerSystem : public S::AStatusMonoSystem<C::TimerPool> {
public:
    ~IncrementTimerSystem() override = default;

    void getRunStepData(SystemTree &sysTree) override;
    float deltaTime = 0.0F;

protected:
    void _statusOperate(typename C::TimerPool::Types &ctimer) override;
};

}