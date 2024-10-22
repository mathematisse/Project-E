/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "Decor.hpp"
#include "Tower.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include <iomanip>
#include <vector>
#include <string>
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

DrawSpriteSystem::DrawSpriteSystem(AssetsLoader &assetsLoader):
    AMonoSystem(false),
    assetsLoader(assetsLoader)
{
}

void DrawSpriteSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::SizePool::Types &csize, C::SpritePool::Types &csprite
)
{
    auto [status] = cstatus;
    if (status != C::EntityStatusEnum::ENT_ALIVE) {
        return;
    }
    auto [id] = csprite;
    if (id == 0) {
        return;
    }
    auto [x, y] = cposition;
    auto [sizeX, sizeY] = csize;
    auto texture = assetsLoader.get_asset_from_id(id);
    float scale = texture.width / sizeX;
    float rotation = 0;
    float adjustedX = x;
    float adjustedY = y;
    if (rotation == 90) {
        adjustedX += sizeX;
    }
    if (rotation == -90) {
        adjustedY += sizeY;
    }
    DrawTextureEx(texture, {adjustedX, adjustedY}, rotation, 1 / scale, WHITE);
}

TowerClickSystem::TowerClickSystem():
    AMonoSystem(false)
{
}

void TowerClickSystem::_innerOperate(
    C::PositionPool::Types &cposition, C::SizePool::Types &csize, C::TypePool::Types &ctype,
    C::LevelPool::Types &clevel, C::IDPool::Types &cid
)
{
    auto [x, y] = cposition;
    auto [sizeX, sizeY] = csize;
    auto [temp_id] = cid;
    auto [level] = clevel;
    auto [type] = ctype;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        auto mousePos = GetMousePosition();
        if (mousePos.x >= x && mousePos.x <= x + sizeX && mousePos.y >= y &&
            mousePos.y <= y + sizeY) {
            open = true;
            pos.x = x;
            pos.y = y;
            selectedTower.id = temp_id;
            selectedTower.level = level;
            selectedTower.type = type;
        }
    }

    if (open && selectedTower.id == temp_id) {
        level = selectedTower.level;
    }
}

} // namespace S
} // namespace ECS