/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "ClientSystems.hpp"
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

MovePlayerSystem::MovePlayerSystem():
    AStatusMonoSystem(false, C::ENT_ALIVE)
{
}

void MovePlayerSystem::_statusOperate(C::VelocityPool::Types &cvelocity, C::TypePool::Types &ctype)
{
    auto [type] = ctype;
    if (type != SquareType::PLAYER) {
        return;
    }
    auto [vX, vY, _] = cvelocity;

    vX = 0;
    vY = 0;
    if (IsKeyDown(KEY_UP)) {
        vY -= 1;
    }
    if (IsKeyDown(KEY_DOWN)) {
        vY += 1;
    }
    if (IsKeyDown(KEY_LEFT)) {
        vX -= 1;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        vX += 1;
    }
}

UpdateEnginePosition::UpdateEnginePosition():
    AMonoSystem(false)
{
}

void UpdateEnginePosition::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition, C::TypePool::Types &ctype
)
{
    auto [engine_status] = cstatus;
    auto [x, y] = cposition;
    auto [type] = ctype;
    if (type == SquareType::ENGINE) {
        if (engine_status != C::EntityStatusEnum::ENT_ALIVE) {
            return;
        }
        if (playerAlive == 0) {
            engine_status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
            return;
        }
        x = playerPosition.x + 80;
        y = playerPosition.y;
    }
}

} // namespace S
} // namespace ECS