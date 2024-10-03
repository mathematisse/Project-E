/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Demo/Systems.hpp"
#include "Components/PureComponentPools.hpp"
#include "Demo/ExamplePoint.hpp"
#include <cmath>
#include <iostream>
#include <iomanip>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

namespace ECS
{
    // SYSTEM

    MoveUpSystem::MoveUpSystem()
        : AMonoSystem(false)
    {
    }


    void MoveUpSystem::_innerOperate(Components::EntityStatusPool::Types& cstatus, Components::PositionPool::Types& cposition, Components::VelocityPool::Types& cvelocity)
    {
        auto [status] = cstatus;
        if ( status != Components::EntityStatusEnum::ENT_ALIVE) {
            return;
        }
        auto [x, y] = cposition;
        auto [vX, vY] = cvelocity;
        x += vX;
        y += vY;
    }

    InitSystem::InitSystem(float velocityMin, float velocityMax)
        : AMonoSystem(false), _velocityMin(velocityMin), _velocityMax(velocityMax)
    {
    }


    void InitSystem::_innerOperate(Components::EntityStatusPool::Types& cstatus, Components::VelocityPool::Types& cvelocity)
    {
        auto [status] = cstatus;
        if ( status != Components::EntityStatusEnum::ENT_NEEDS_INIT) {
            return;
        }
        std::cout << YELLOW "Initializing...\n" RESET;
        auto [x, y] = cvelocity;
        x += _velocityMin + (rand() % (int)(_velocityMax - _velocityMin));
        y += _velocityMin + (rand() % (int)(_velocityMax - _velocityMin));
        status = Components::EntityStatusEnum::ENT_ALIVE;
    }

    PrintSystem::PrintSystem()
        : AMonoSystem(false)
    {
    }


    void PrintSystem::_innerOperate(Components::EntityStatusPool::Types& cstatus, Components::PositionPool::Types& cposition)
    {
        std::cout << RESET;
        auto [status] = cstatus;
        if ( status != Components::EntityStatusEnum::ENT_ALIVE) {
            std::cout << RED "{--------}" RESET;
            return;
        }
        auto [x, y] = cposition;
        std::cout << GREEN << "{" << std::fixed << std::setprecision(1) << x << ", " << std::fixed << std::setprecision(1) << y << "}" << RESET;
    }

    DualExampleSystem::DualExampleSystem()
        : ADualSystem(false)
    {
    }

    void DualExampleSystem::_innerOperate(
        Components::EntityStatusPool::Types& cStatusA,
        Components::ChunkPosPool::Types& cChunkPosA,
        Components::EntityStatusPool::Types& cStatusB,
        Components::ChunkPosPool::Types& cChunkPosB)
    {
        auto [statusA] = cStatusA;

        auto [statusB] = cStatusB;

        if ( statusA != Components::EntityStatusEnum::ENT_ALIVE || statusB != Components::EntityStatusEnum::ENT_ALIVE) {
            std::cout << "x";
            return;
        }
        if (cChunkPosA == cChunkPosB) {
            std::cout << "o";
            return;
        }
        std::cout << "-";
    }

}