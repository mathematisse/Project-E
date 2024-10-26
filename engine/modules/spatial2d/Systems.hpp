#pragma once

#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "Components.hpp"

namespace ECS::S {

class ApplyVelocitySystem : public S::AStatusMonoSystem<C::PositionPool, C::VelocityPool>,
                            public S::ADeltaTimeSystem {
public:
    ApplyVelocitySystem() = default;
    ~ApplyVelocitySystem() override = default;

protected:
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::VelocityPool::Types &cvelocity
    ) override;
};

//////////////////////////////////////////////////////////////////////////////////

class ApplyAccelerationSystem : public S::AStatusMonoSystem<C::VelocityPool, C::AccelerationPool>,
                                public S::ADeltaTimeSystem {
public:
    ~ApplyAccelerationSystem() override = default;

protected:
    void _statusOperate(
        typename C::VelocityPool::Types &cvelocity,
        typename C::AccelerationPool::Types &cacceleration
    ) override;
};

}
