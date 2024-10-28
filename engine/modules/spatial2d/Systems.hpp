#pragma once

#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "Components.hpp"

namespace ECS::S {

class ApplyVelocitySystem : public S::AMonoSystem<C::Position::Pool, C::Velocity::Pool>,
                            public S::ADeltaTimeSystem {
public:
    ~ApplyVelocitySystem() override = default;

protected:
    void _innerOperate(
        typename C::Position::Pool::Types &cposition, typename C::Velocity::Pool::Types &cvelocity
    ) override;
};

//////////////////////////////////////////////////////////////////////////////////

class ApplyAccelerationSystem : public S::AMonoSystem<C::Velocity::Pool, C::Acceleration::Pool>,
                                public S::ADeltaTimeSystem {
public:
    ~ApplyAccelerationSystem() override = default;

protected:
    void _innerOperate(
        typename C::Velocity::Pool::Types &cvelocity,
        typename C::Acceleration::Pool::Types &cacceleration
    ) override;
};

}
