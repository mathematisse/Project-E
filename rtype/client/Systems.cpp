/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "Square.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include <iomanip>
#include <iostream>
#include <raylib.h>

#define RED_CLI "\033[31m"
#define GREEN_CLI "\033[32m"
#define YELLOW_CLI "\033[33m"
#define RESET "\033[0m"

namespace ECS {
namespace S {
// SYSTEM

DrawSystem::DrawSystem():
    AMonoSystem(false)
{
}

void DrawSystem::_innerOperate(C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition)
{
    auto [status] = cstatus;
    if (status != C::EntityStatusEnum::ENT_ALIVE) {
        return;
    }
    auto [x, y] = cposition;
    std::cout << "Drawing square at [" << x << ", " << y << "]\n";
    DrawRectangle((int)x, (int)y, 50, 50, {255, 0, 0, 255});
}

ApplyVelocitySystem::ApplyVelocitySystem():
    AMonoSystem(false)
{
}

void ApplyVelocitySystem::_innerOperate(C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity)
{
    auto [x, y] = cposition;
    auto [vX, vY, speed] = cvelocity;
    x += vX * deltaTime * speed;
    y += vY * deltaTime * speed;
}

MovePlayerSystem::MovePlayerSystem():
    AMonoSystem(false)
{
}

void MovePlayerSystem::_innerOperate(C::EntityStatusPool::Types &cstatus, C::VelocityPool::Types &cvelocity, C::TypePool::Types &ctype)
{
    auto [status] = cstatus;
    auto [type] = ctype;
    if (status != C::EntityStatusEnum::ENT_ALIVE || type != SquareType::PLAYER) {
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

} // namespace S
} // namespace ECS