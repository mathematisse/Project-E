/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "DrawSystems.hpp"
#include "Components.hpp"
#include "AssetsPath.hpp"
#include <raylib.h>

namespace ECS {
namespace S {
// SYSTEM

ShowInfoSystem::ShowInfoSystem(Camera2D &camera):
    AMonoSystem(false),
    camera(camera)
{
}

void ShowInfoSystem::_innerOperate(C::Type::Pool::Types &ctype, C::Health::Pool::Types &chealth)
{
    auto [type] = ctype;

    if (type != GameEntityType::LPLAYER) {
        return;
    }
    auto [health] = chealth;
    Vector2 top_left = {camera.target.x - 1920 / 2, camera.target.y - 1080 / 2};
    DrawText("Player health: ", top_left.x + 10, top_left.y + 10, 20, RED);
    DrawText(std::to_string(health).c_str(), top_left.x + 200, top_left.y + 10, 20, RED);
}

} // namespace S
} // namespace ECS