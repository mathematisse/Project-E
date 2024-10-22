/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "DrawSystems.hpp"
#include "Decor.hpp"
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

} // namespace S
} // namespace ECS