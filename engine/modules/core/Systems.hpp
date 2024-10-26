#pragma once

#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "Components.hpp"
#include "lib_ecs/Systems/ASystem.hpp"

namespace ECS::S {

class IncrementTimerSystem : public S::AStatusMonoSystem<C::TimerPool>, public S::ADeltaTimeSystem {
public:
    ~IncrementTimerSystem() override = default;

protected:
    void _statusOperate(typename C::TimerPool::Types &ctimer) override;
};

}