/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "Decor.hpp"
#include "Tower.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include <iomanip>
#include <vector>
#include <string>
#include <iostream>
#include "AssetsPath.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include <raylib.h>
#include "math.h"

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
    if (sizeX == 70 && sizeY == 70) {
        return;
    }
    auto texture = assetsLoader.get_asset_from_id(id);
    float scale = texture.width / sizeX;
    float rotation = 0;
    if (scale <= 0) {
        scale = 1;
    }
    DrawTextureEx(texture, {x, y}, rotation, 1 / scale, WHITE);
}

DrawRotationProjectileSystem::DrawRotationProjectileSystem(AssetsLoader &assetsLoader):
    AMonoSystem(false),
    assetsLoader(assetsLoader)
{
}

void DrawRotationProjectileSystem::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::SizePool::Types &csize, C::SpritePool::Types &csprite, C::VelocityPool::Types &cvelocity,
    C::DestPool::Types &cdest
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
    auto [velocityX, velocityY] = cvelocity;

    auto texture = assetsLoader.get_asset_from_id(id);
    float scale = texture.width / sizeX;

    float rotation = atan2(velocityY, velocityX) * (180.0 / M_PI);

    float centerX = x + sizeX / 2;
    float centerY = y + sizeY / 2;

    if (scale <= 0) {
        scale = 1;
    }
    DrawTexturePro(
        texture, {0, 0, (float) texture.width, (float) texture.height},
        {centerX, centerY, (float) sizeX, (float) sizeY}, {(float) sizeX / 2, (float) sizeY / 2},
        rotation, WHITE
    );
}

ApplyVelocitySystem::ApplyVelocitySystem():
    AStatusMonoSystem(true, C::ENT_ALIVE)
{
}

void ApplyVelocitySystem::_statusOperate(
    C::PositionPool::Types &cposition, C::VelocityPool::Types &cvelocity
)
{
    auto [x, y] = cposition;
    auto [vX, vY] = cvelocity;
    int speed = 100;

    x += vX * deltaTime * speed;
    y += vY * deltaTime * speed;
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
        if (mousePos.x >= x && mousePos.x <= x + (sizeX * 1.5) && mousePos.y >= y &&
            mousePos.y <= y + (sizeY * 1.5)) {
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
        type = selectedTower.type;
    }
    if (type == WIZARD && sizeX == 75) {
        sizeX = 100;
        sizeY = 100;
    }
}

ChangeTowerSprite::ChangeTowerSprite(AssetsLoader &assetsLoader):
    AMonoSystem(false),
    assetsLoader(assetsLoader)
{
}

void ChangeTowerSprite::_innerOperate(
    C::SpritePool::Types &csprite, C::TypePool::Types &ctype, C::LevelPool::Types &clevel,
    C::IDPool::Types &cid
)
{
    auto [type] = ctype;
    auto [level] = clevel;
    auto [sprite] = csprite;

    if (level == 1) {
        if (type == ARCHER) {
            auto texture = assetsLoader.get_asset(ARCHER1_TOWER);
            sprite = texture.id;
        }
        if (type == WIZARD) {
            auto texture = assetsLoader.get_asset(WIZARD1_TOWER);
            sprite = texture.id;
        }
    } else if (level == 2) {
        if (type == ARCHER) {
            auto texture = assetsLoader.get_asset(ARCHER2_TOWER);
            sprite = texture.id;
        }
        if (type == WIZARD) {
            auto texture = assetsLoader.get_asset(WIZARD2_TOWER);
            sprite = texture.id;
        }
    } else if (level == 3) {
        if (type == ARCHER) {
            auto texture = assetsLoader.get_asset(ARCHER3_TOWER);
            sprite = texture.id;
        }
        if (type == WIZARD) {
            auto texture = assetsLoader.get_asset(WIZARD3_TOWER);
            sprite = texture.id;
        }
    }
}

SpawnEnemy::SpawnEnemy(AssetsLoader &assetsLoader, EntityManager &_eM):
    AMonoSystem(false),
    assetsLoader(assetsLoader),
    _eM(_eM)
{
}

void SpawnEnemy::_innerOperate(C::EntityStatusPool::Types &cstatus, C::ScorePool::Types &cscore)
{
    auto [status] = cstatus;
    if (status != C::EntityStatusEnum::ENT_ALIVE) {
        return;
    }
    auto [score] = cscore;
    score += kills;

    float spawn_rate = (5.0f - (score / 10.0f));

    if (spawn_rate < 0.5) {
        spawn_rate = 0.5;
    }
    if (delay < spawn_rate) {
        return;
    }
    delay = 0;
    auto enemies = _eM.createEntities("Enemy", 1, ECS::C::ENT_ALIVE);

    std::vector<Vector2> spawns = {{2020, 900}, {1305, -100}};
    Vector2 spawn = spawns[rand() % 2];

    for (const auto &entity : enemies) {
        auto enemyRef = _eM.getEntity(entity);

        auto square_enemy = dynamic_cast<ECS::E::EnemyRef *>(enemyRef.get());
        if (!square_enemy) {
            std::cerr << "Failed to cast IEntityRef to EnemyRef" << std::endl;
            return;
        }
        square_enemy->getPosition()->set<0>(spawn.x);
        square_enemy->getPosition()->set<1>(spawn.y);
        square_enemy->getSize()->set<0>(50);
        square_enemy->getSize()->set<1>(50);
        square_enemy->getSprite()->set<0>(assetsLoader.get_asset(GOBLIN).id);
        square_enemy->getHealth()->set<0>(100);
        square_enemy->getVelocity()->set<0>(0);
        square_enemy->getVelocity()->set<1>(0);
        square_enemy->getHealth()->set<0>(20);
    }
}

static Vector2 go_to_pos(Vector2 dest, Vector2 origin)
{
    Vector2 dir = {dest.x - origin.x, dest.y - origin.y};
    float mag = std::sqrt(std::pow(dir.x, 2) + std::pow(dir.y, 2));
    dir.x /= mag;
    dir.y /= mag;
    return dir;
}

static const std::vector<Vector2> path1 = {{1738, 879}, {1506, 743}, {1375, 760},
                                           {1225, 863}, {1045, 895}, {830, 782},
                                           {810, 680},  {750, 411},  {-75, 409}};
static const std::vector<Vector2> path2 = {{1229, 112}, {949, 158}, {886, 171},
                                           {842, 210},  {742, 409}, {-75, 409}};

MoveEnemy::MoveEnemy():
    AMonoSystem(false)
{
}

void MoveEnemy::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::VelocityPool::Types &cvelocity, C::HealthPool::Types &chealth
)
{
    auto [x, y] = cposition;
    auto [vX, vY] = cvelocity;
    auto [status] = cstatus;

    if (status != C::EntityStatusEnum::ENT_ALIVE) {
        return;
    }

    if (x <= -75) {
        status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        player_health--;
        return;
    }

    vX = 0;
    vY = 0;
    size_t i = 0;
    std::vector<Vector2> path = {{0, 0}};

    if (y > 409) {
        path = path1;
    } else {
        path = path2;
    }
    while (path[i].x >= x) {
        i++;
        if (i >= path.size()) {
            return;
        }
    }
    Vector2 dir = go_to_pos(path[i], {x, y});
    vX = dir.x;
    vY = dir.y;
}

void shoot_projectile(
    AssetsLoader &assetsLoader, EntityManager &_eM, tower_info &tower, Vector2 dest
)
{
    auto projectiles = _eM.createEntities("Projectile", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : projectiles) {
        auto projectileRef = _eM.getEntity(entity);

        auto square_projectile = dynamic_cast<ECS::E::ProjectileRef *>(projectileRef.get());
        if (!square_projectile) {
            std::cerr << "Failed to cast IEntityRef to ProjectileRef" << std::endl;
            return;
        }
        square_projectile->getPosition()->set<0>(tower.pos.x + 50);
        square_projectile->getPosition()->set<1>(tower.pos.y + 50);
        square_projectile->getSize()->set<0>(70);
        square_projectile->getSize()->set<1>(70);
        if (tower.type == ARCHER) {
            square_projectile->getSprite()->set<0>(assetsLoader.get_asset(ARROW).id);
        } else if (tower.type == WIZARD) {
            square_projectile->getSprite()->set<0>(assetsLoader.get_asset(FIREBALL).id);
        }
        Vector2 vel = go_to_pos(dest, {tower.pos.x + 50, tower.pos.y + 50});
        square_projectile->getVelocity()->set<0>(vel.x * 5);
        square_projectile->getVelocity()->set<1>(vel.y * 5);
        square_projectile->getDest()->set<0>(dest);
    }
}

DamageEnemy::DamageEnemy(AssetsLoader &assetsLoader, EntityManager &_eM):
    AMonoSystem(false),
    assetsLoader(assetsLoader),
    _eM(_eM)
{
}

void DamageEnemy::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::SizePool::Types &csize, C::HealthPool::Types &chealth
)
{
    auto [x, y] = cposition;
    auto [health] = chealth;
    auto [status] = cstatus;

    if (status != C::EntityStatusEnum::ENT_ALIVE) {
        return;
    }

    if (health <= 0) {
        status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
        kills++;
        money += 20;
        return;
    }

    for (auto &tower : towers) {
        if (tower.type == NONE) {
            continue;
        }
        float dx = tower.pos.x - x;
        float dy = tower.pos.y - y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= tower_range[tower.type][tower.level - 1]) {
            if (tower.type == ARCHER) {
                if (tower.delay > 1) {
                    shoot_projectile(assetsLoader, _eM, tower, {x - 50, y});
                    health -= 5;
                    tower.delay = 0;
                }
            } else if (tower.type == WIZARD) {
                if (tower.delay > 2.5) {
                    shoot_projectile(assetsLoader, _eM, tower, {x - 50, y});
                    health -= 15;
                    tower.delay = 0;
                }
            }
        }
    }
}

KillProjectile::KillProjectile():
    AMonoSystem(false)
{
}

void KillProjectile::_innerOperate(
    C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition,
    C::VelocityPool::Types &cvelocity, C::DestPool::Types &cdest
)
{
    auto [status] = cstatus;
    auto [x, y] = cposition;
    auto [velx, vely] = cvelocity;
    bool x_done = false;
    bool y_done = false;

    if (status != C::EntityStatusEnum::ENT_ALIVE) {
        return;
    }

    auto [dest] = cdest;

    if (velx > 0) {
        if (x >= dest.x) {
            x_done = true;
        }
    } else {
        if (x <= dest.x) {
            x_done = true;
        }
    }
    if (vely > 0) {
        if (y >= dest.y) {
            y_done = true;
        }
    } else {
        if (y <= dest.y) {
            y_done = true;
        }
    }

    if (x_done && y_done) {
        status = C::EntityStatusEnum::ENT_NEEDS_DESTROY;
    }
}

} // namespace S
} // namespace ECS