#include "Systems.hpp"

namespace ECS::S {

void ApplyVelocitySystem::_statusOperate(
    C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity
)
{
    auto [x, y] = cposition;
    auto [vX, vY, speed] = cvelocity;
    x += vX * deltaTime * speed;
    y += vY * deltaTime * speed;
}

//////////////////////////////////////////////////////////////////////////////////

void ApplyAccelerationSystem::_statusOperate(
    C::VelocityPool::Types &cvelocity, C::AccelerationPool::Types &cacceleration
)
{
    auto [vX, vY, speed] = cvelocity;
    auto [aX, aY] = cacceleration;
    vX += aX * deltaTime;
    vY += aY * deltaTime;
}

}
