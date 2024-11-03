#include "Systems.hpp"
#include "lib_ecs/Systems/ExecutionTypes.hpp"

namespace ECS::S {

ApplyVelocitySystem::ApplyVelocitySystem():
    AMonoSystem(PARALLEL_SYSTEM_EXECUTION)
{
}

void ApplyVelocitySystem::_innerOperate(
    C::Position::Pool::Types &cposition, C::Velocity::Pool::Types &cvelocity
)
{
    auto [x, y] = cposition;
    auto [vX, vY] = cvelocity;
    x += vX * _deltaTime;
    y += vY * _deltaTime;
}

//////////////////////////////////////////////////////////////////////////////////

ApplyAccelerationSystem::ApplyAccelerationSystem():
    AMonoSystem(PARALLEL_SYSTEM_EXECUTION)
{
}

void ApplyAccelerationSystem::_innerOperate(
    C::Velocity::Pool::Types &cvelocity, C::Acceleration::Pool::Types &cacceleration
)
{
    auto [vX, vY] = cvelocity;
    auto [aX, aY] = cacceleration;
    vX += aX * _deltaTime;
    vY += aY * _deltaTime;
}

}