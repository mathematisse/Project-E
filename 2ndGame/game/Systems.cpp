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
    C::Rotation::Pool::Types &crotation, C::Velocity::Pool::Types &cvelocity,
    C::Dest::Pool::Types & /*cdest*/
)
{
    auto [vX, vY] = cvelocity;
    auto [rotation] = crotation;

    rotation = (float) (std::atan2(vY, vX) * (180.0 / M_PI));
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
    if (type == TowerType::WIZARD && sizeX == 120) {
        sizeX = 140;
        sizeY = 140;
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

    if (level == 0) {
        return;
    }
    sprite = spriteIds[(level - 1) * 2 + (int) (type == TowerType::WIZARD)];
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
    enemy.setSize({65, 65});
    enemy.setSprite(spriteId);
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

static const std::vector<Vector2> path1 = {{1788, 929}, {1556, 793}, {1425, 810},
                                           {1275, 913}, {1095, 945}, {880, 832},
                                           {860, 730},  {800, 461},  {-25, 459}};
static const std::vector<Vector2> path2 = {{1279, 162}, {999, 208}, {936, 221},
                                           {892, 260},  {792, 459}, {-25, 459}};

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
    vX = dir.x * 100.0F;
    vY = dir.y * 100.0F;
}

void shoot_projectile(
    AssetsLoader &assetsLoader, EntityManager &_eM, tower_info &tower, Vector2 dest
)
{
    auto projectile = _eM.createEntity<E::ProjectileEntity>();
    projectile.setPosition({tower.pos.x, tower.pos.y});
    projectile.setSize({80, 80});
    if (tower.type == TowerType::ARCHER) {
        projectile.setSprite({assetsLoader.get_asset(ARROW).id});
    } else if (tower.type == TowerType::WIZARD) {
        projectile.setSprite({assetsLoader.get_asset(FIREBALL).id});
    }
    Vector2 vel = go_to_pos(dest, {tower.pos.x, tower.pos.y});
    projectile.setVelocity({vel.x * 500.0F, vel.y * 500.0F});
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
                shoot_projectile(assetsLoader, _eM, tower, {x, y});
                health -= 5;
                tower.delay = 0;
            }
        } else if (tower.type == TowerType::WIZARD) {
            if (tower.delay > 2.5) {
                shoot_projectile(assetsLoader, _eM, tower, {x, y});
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

DestroyEntitiesSystem::DestroyEntitiesSystem(EntityManager &entityManager):
    AStatusMonoSystem(false, C::ENT_NEEDS_DESTROY),
    entityManager(entityManager)
{
}

void DestroyEntitiesSystem::_statusOperate(C::ChunkPos::Pool::Types &cchunkpos)
{
    entityManager.destroyEntity(cchunkpos);
}

} // namespace S
} // namespace ECS