#pragma once

#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "Components.hpp"
#include "lib_ecs/Systems/ASystem.hpp"

namespace ECS::S {

class IncrementTimerSystem : public S::AMonoSystem<C::Timer::Pool>, public S::ADeltaTimeSystem {
public:
    ~IncrementTimerSystem() override = default;

protected:
    void _innerOperate(typename C::Timer::Pool::Types &ctimer) override;
};

}