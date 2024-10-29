/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Systems.hpp"
#include "Archetypes.hpp"
#include "Components.hpp"
#include "render/AssetsLoader.hpp"
#include <cmath>
#include <vector>
#include "AssetsPath.hpp"
#include <raylib.h>

namespace ECS {
namespace S {
// SYSTEM

DrawRotationProjectileSystem::DrawRotationProjectileSystem(AssetsLoader &assetsLoader):
    AMonoSystem(false),
    assetsLoader(assetsLoader)
{
}

void DrawRotationProjectileSystem::_innerOperate(
    C::EntityStatus::Pool::Types &cstatus, C::Position::Pool::Types &cposition,
    C::Size::Pool::Types &csize, C::Sprite::Pool::Types &csprite,
    C::Velocity::Pool::Types &cvelocity, C::Dest::Pool::Types & /*cdest*/
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
    float scale = (float) texture.width / sizeX;

    auto rotation = (float) (std::atan2(velocityY, velocityX) * (180.0 / M_PI));

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

TowerClickSystem::TowerClickSystem():
    AMonoSystem(false)
{
}

void TowerClickSystem::_innerOperate(
    C::Position::Pool::Types &cposition, C::Size::Pool::Types &csize, C::Type::Pool::Types &ctype,
    C::Level::Pool::Types &clevel, C::ID::Pool::Types &cid
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
    if (type == TowerType::WIZARD && sizeX == 75) {
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
    C::Sprite::Pool::Types &csprite, C::Type::Pool::Types &ctype, C::Level::Pool::Types &clevel,
    C::ID::Pool::Types & /*cid*/
)
{
    auto [type] = ctype;
    auto [level] = clevel;
    auto [sprite] = csprite;

    if (level == 1) {
        if (type == TowerType::ARCHER) {
            auto texture = assetsLoader.get_asset(ARCHER1_TOWER);
            sprite = texture.id;
        }
        if (type == TowerType::WIZARD) {
            auto texture = assetsLoader.get_asset(WIZARD1_TOWER);
            sprite = texture.id;
        }
    } else if (level == 2) {
        if (type == TowerType::ARCHER) {
            auto texture = assetsLoader.get_asset(ARCHER2_TOWER);
            sprite = texture.id;
        }
        if (type == TowerType::WIZARD) {
            auto texture = assetsLoader.get_asset(WIZARD2_TOWER);
            sprite = texture.id;
        }
    } else if (level == 3) {
        if (type == TowerType::ARCHER) {
            auto texture = assetsLoader.get_asset(ARCHER3_TOWER);
            sprite = texture.id;
        }
        if (type == TowerType::WIZARD) {
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

void SpawnEnemy::_innerOperate(C::EntityStatus::Pool::Types &cstatus, C::Score::Pool::Types &cscore)
{
    auto [status] = cstatus;
    if (status != C::EntityStatusEnum::ENT_ALIVE) {
        return;
    }
    auto [score] = cscore;
    score += kills;

    float spawn_rate = (5.0F - ((float) score / 10.0F));

    if (spawn_rate < 0.5) {
        spawn_rate = 0.5;
    }
    if (delay < spawn_rate) {
        return;
    }
    delay = 0;
    auto enemy = _eM.createEntity<ECS::E::EnemyEntity>();

    std::vector<Vector2> spawns = {{2020, 900}, {1305, -100}};
    Vector2 spawn = spawns[rand() % 2];
    enemy.setPosition({spawn.x, spawn.y});
    enemy.setSize({50, 50});
    enemy.setSprite({assetsLoader.get_asset(GOBLIN).id});
    enemy.setHealth({20});
    enemy.setVelocity({0.0F, 0.0F});
}

static Vector2 go_to_pos(Vector2 dest, Vector2 origin)
{
    Vector2 dir = {dest.x - origin.x, dest.y - origin.y};
    auto mag = (float) std::sqrt(std::pow(dir.x, 2) + std::pow(dir.y, 2));
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
    C::EntityStatus::Pool::Types &cstatus, C::Position::Pool::Types &cposition,
    C::Velocity::Pool::Types &cvelocity, C::Health::Pool::Types & /*chealth*/
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
    auto projectile = _eM.createEntity<E::ProjectileEntity>();
    projectile.setPosition({tower.pos.x + 50, tower.pos.y + 50});
    projectile.setSize({70, 70});
    if (tower.type == TowerType::ARCHER) {
        projectile.setSprite({assetsLoader.get_asset(ARROW).id});
    } else if (tower.type == TowerType::WIZARD) {
        projectile.setSprite({assetsLoader.get_asset(FIREBALL).id});
    }
    Vector2 vel = go_to_pos(dest, {tower.pos.x + 50, tower.pos.y + 50});
    projectile.setVelocity({vel.x * 5, vel.y * 5});
    projectile.setDest({dest});
}

DamageEnemy::DamageEnemy(AssetsLoader &assetsLoader, EntityManager &_eM):
    AMonoSystem(false),
    assetsLoader(assetsLoader),
    _eM(_eM)
{
}

void DamageEnemy::_innerOperate(
    C::EntityStatus::Pool::Types &cstatus, C::Position::Pool::Types &cposition,
    C::Size::Pool::Types & /*csize*/, C::Health::Pool::Types &chealth
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
        if (tower.type == TowerType::NONE) {
            continue;
        }
        float dx = tower.pos.x - x;
        float dy = tower.pos.y - y;
        auto distance = (float) std::sqrt(dx * dx + dy * dy);

        if (distance > (float) tower_range[(size_t) tower.type][tower.level - 1]) {
            continue;
        }

        if (tower.type == TowerType::ARCHER) {
            if (tower.delay > 1) {
                shoot_projectile(assetsLoader, _eM, tower, {x - 50, y});
                health -= 5;
                tower.delay = 0;
            }
        } else if (tower.type == TowerType::WIZARD) {
            if (tower.delay > 2.5) {
                shoot_projectile(assetsLoader, _eM, tower, {x - 50, y});
                health -= 15;
                tower.delay = 0;
            }
        }
    }
}

KillProjectile::KillProjectile():
    AMonoSystem(false)
{
}

void KillProjectile::_innerOperate(
    C::EntityStatus::Pool::Types &cstatus, C::Position::Pool::Types &cposition,
    C::Velocity::Pool::Types &cvelocity, C::Dest::Pool::Types &cdest
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