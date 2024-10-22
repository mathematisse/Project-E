/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Decor.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "AssetsLoader.hpp"
#include "raylib.h"

namespace ECS::S {

class DrawSpriteSystem
    : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::SizePool, C::SpritePool> {
public:
    explicit DrawSpriteSystem(AssetsLoader &assetsLoader);
    ~DrawSpriteSystem() override = default;

    DrawSpriteSystem(const DrawSpriteSystem &other) = default;
    DrawSpriteSystem(DrawSpriteSystem &&other) = default;
    DrawSpriteSystem &operator=(const DrawSpriteSystem &other) = default;
    DrawSpriteSystem &operator=(DrawSpriteSystem &&other) = default;

    AssetsLoader &assetsLoader;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::SizePool::Types &csize, typename C::SpritePool::Types &csprite
    ) override;
};

} // namespace ECS::S
