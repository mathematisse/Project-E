/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "NetSystems.hpp"
#include "DecorSquare.hpp"
#include "Square.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include <iomanip>
#include <iostream>
#include "AssetsPath.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include <raylib.h>

#define RED_CLI "\033[31m"
#define GREEN_CLI "\033[32m"
#define YELLOW_CLI "\033[33m"
#define RESET "\033[0m"

namespace ECS {
namespace S {
// SYSTEM

SendDecorStateSystem::SendDecorStateSystem():
    AMonoSystem(false)
{
}

void SendDecorStateSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::SizePool::Types &csize, C::TypePool::Types &ctype, C::SpritePool::Types &csprite,
    C::NetworkIDPool::Types &cnetworkID
)
{
    auto [status] = cstatus;
    auto [x, y] = cposition;
    auto [sizeX, sizeY, _] = csize;
    auto [type] = ctype;
    auto [id, flag, sprite_x, sprite_y, nbr_frame, start_position, animation_time] = csprite;
    auto [netid] = cnetworkID;

    if (type != SquareType::BACKGROUND && type != SquareType::WALL) {
        return;
    }
}

SendSquareStateSystem::SendSquareStateSystem():
    AMonoSystem(false)
{
}

void SendSquareStateSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::VelocityPool::Types &cvelocity, C::ColorPool::Types &ccolor, C::SizePool::Types &csize,
    C::TypePool::Types &ctype, C::CanShootPool::Types &canshoot, C::SpritePool::Types &csprite,
    C::HealthPool::Types &chealth, C::TimerPool::Types &ctimer, C::NetworkIDPool::Types &cnetworkID
)
{
    auto [status] = cstatus;
    auto [x, y] = cposition;
    auto [vX, vY, speed] = cvelocity;
    auto [r, g, b, a] = ccolor;
    auto [sizeX, sizeY, rotation] = csize;
    auto [type] = ctype;
    auto [canShoot, base_delay, delay] = canshoot;
    auto [id, flag, sprite_x, sprite_y, nbr_frame, start_position, animation_time] = csprite;
    auto [health] = chealth;
    auto [clock, end_clock] = ctimer;
    auto [netid] = cnetworkID;

    if (type == SquareType::BACKGROUND || type == SquareType::WALL) {
        return;
    }
}

} // namespace S
} // namespace ECS