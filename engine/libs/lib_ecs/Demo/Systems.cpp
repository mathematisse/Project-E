/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "lib_ecs/Demo/Systems.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Demo/ExamplePoint.hpp"
#include <iomanip>
#include <iostream>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

namespace ECS {
namespace S {
// SYSTEM

MoveUpSystem::MoveUpSystem():
    AStatusMonoSystem(false, C::ENT_ALIVE)
{
}

void MoveUpSystem::_statusOperate(
    C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity
)
{
    auto [x, y] = cposition;
    auto [vX, vY] = cvelocity;
    x += vX;
    y += vY;
}

InitSystem::InitSystem(float velocityMin, float velocityMax):
    AMonoSystem(false),
    _velocityMin(velocityMin),
    _velocityMax(velocityMax)
{
}

void InitSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::VelocityPool::Types &cvelocity
)
{
    auto [status] = cstatus;
    if (status != C::EntityStatusEnum::ENT_NEEDS_INIT) {
        return;
    }
    std::cout << YELLOW "Initializing...\n" RESET;
    auto [x, y] = cvelocity;
    x += _velocityMin + (rand() % (int) (_velocityMax - _velocityMin));
    y += _velocityMin + (rand() % (int) (_velocityMax - _velocityMin));
    status = C::EntityStatusEnum::ENT_ALIVE;
}

PrintSystem::PrintSystem():
    AMonoSystem(false)
{
}

void PrintSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition
)
{
    std::cout << RESET;
    auto [status] = cstatus;
    if (status != C::EntityStatusEnum::ENT_ALIVE) {
        std::cout << RED "{--------}" RESET;
        return;
    }
    auto [x, y] = cposition;
    std::cout << GREEN << "{" << std::fixed << std::setprecision(1) << x << ", " << std::fixed
              << std::setprecision(1) << y << "}" << RESET;
}

void PrintSystem::
    _endCMapCallBack(C::EntityStatusPool::VTypes & /*unused*/, C::PositionPool::VTypes & /*unused*/)
{
    std::cout << "\n";
}

DualExampleSystem::DualExampleSystem():
    ADualSystem(false)
{
}

void DualExampleSystem::_innerOperate(
    C::EntityStatusPool::Types &cStatusA, C::ChunkPosPool::Types &cChunkPosA,
    C::EntityStatusPool::Types &cStatusB, C::ChunkPosPool::Types &cChunkPosB
)
{
    auto [statusA] = cStatusA;

    auto [statusB] = cStatusB;

    if (statusA != C::EntityStatusEnum::ENT_ALIVE || statusB != C::EntityStatusEnum::ENT_ALIVE) {
        std::cout << "x";
        return;
    }
    if (cChunkPosA == cChunkPosB) {
        std::cout << "o";
        return;
    }
    std::cout << "-";
}

} // namespace S
} // namespace ECS