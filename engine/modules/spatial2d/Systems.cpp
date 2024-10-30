#include "Systems.hpp"

namespace ECS::S {

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