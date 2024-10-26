#include "Systems.hpp"

namespace ECS::S {

void ApplyVelocitySystem::_statusOperate(
    C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity
)
{
    auto [x, y] = cposition;
    auto [vX, vY] = cvelocity;
    x += vX * _deltaTime;
    y += vY * _deltaTime;
}

//////////////////////////////////////////////////////////////////////////////////

void ApplyAccelerationSystem::_statusOperate(
    C::VelocityPool::Types &cvelocity, C::AccelerationPool::Types &cacceleration
)
{
    auto [vX, vY] = cvelocity;
    auto [aX, aY] = cacceleration;
    vX += aX * _deltaTime;
    vY += aY * _deltaTime;
}

}