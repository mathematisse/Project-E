/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Decor.hpp"
#include "Tower.hpp"
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

struct tower {
    size_t id;
    size_t level;
    size_t type;
};

class TowerClickSystem
    : public S::AMonoSystem<C::PositionPool, C::SizePool, C::TypePool, C::LevelPool, C::IDPool> {
public:
    explicit TowerClickSystem();
    ~TowerClickSystem() override = default;

    TowerClickSystem(const TowerClickSystem &other) = default;
    TowerClickSystem(TowerClickSystem &&other) = default;
    TowerClickSystem &operator=(const TowerClickSystem &other) = default;
    TowerClickSystem &operator=(TowerClickSystem &&other) = default;

    bool open = false;
    Vector2 pos = {0, 0};
    tower selectedTower = {0, 0, 0};

protected:
    void _innerOperate(
        typename C::PositionPool::Types &cposition, typename C::SizePool::Types &csize,
        typename C::TypePool::Types &ctype, typename C::LevelPool::Types &clevel,
        typename C::IDPool::Types &cid
    ) override;
};

} // namespace ECS::S
