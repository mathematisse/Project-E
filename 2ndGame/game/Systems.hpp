/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Components.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "spatial2d/Components.hpp"
#include "render/Components.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "render/AssetsLoader.hpp"
#include "raylib.h"
#include <tuple>

namespace ECS::S {

class DrawRotationProjectileSystem
    : public S::AMonoSystem<C::Rotation::Pool, C::Velocity::Pool, C::Dest::Pool> {
public:
    explicit DrawRotationProjectileSystem(AssetsLoader &assetsLoader);
    ~DrawRotationProjectileSystem() override = default;

    AssetsLoader &assetsLoader;

protected:
    void _innerOperate(
        typename C::Rotation::Pool::Types &csprite, typename C::Velocity::Pool::Types &cvelocity,
        typename C::Dest::Pool::Types &cdest
    ) override;
};

struct tower {
    size_t id;
    size_t level;
    TowerType type;
};

class TowerClickSystem
    : public S::AMonoSystem<
          C::Position::Pool, C::Size::Pool, C::Type::Pool, C::Level::Pool, C::ID::Pool> {
public:
    explicit TowerClickSystem();
    ~TowerClickSystem() override = default;

    bool open = false;
    Vector2 pos = {0, 0};
    tower selectedTower = {0, 0, TowerType::NONE};

protected:
    void _innerOperate(
        typename C::Position::Pool::Types &cposition, typename C::Size::Pool::Types &csize,
        typename C::Type::Pool::Types &ctype, typename C::Level::Pool::Types &clevel,
        typename C::ID::Pool::Types &cid
    ) override;
};

class ChangeTowerSprite
    : public S::AMonoSystem<C::Sprite::Pool, C::Type::Pool, C::Level::Pool, C::ID::Pool> {
public:
    explicit ChangeTowerSprite(AssetsLoader &assetsLoader);
    ~ChangeTowerSprite() override = default;

    AssetsLoader &assetsLoader;
    std::array<unsigned int, 6> spriteIds = {0, 0, 0, 0, 0, 0};

protected:
    void _innerOperate(
        typename C::Sprite::Pool::Types &csprite, typename C::Type::Pool::Types &ctype,
        typename C::Level::Pool::Types &clevel, typename C::ID::Pool::Types &cid
    ) override;
};

class SpawnEnemy : public S::AMonoSystem<C::EntityStatus::Pool, C::Score::Pool> {
public:
    explicit SpawnEnemy(AssetsLoader &AssetsLoader, EntityManager &_eM);
    ~SpawnEnemy() override = default;

    AssetsLoader &assetsLoader;
    EntityManager &_eM;
    float delay = 0;
    size_t kills = 0;

    unsigned int spriteId = 0;

protected:
    void _innerOperate(
        typename C::EntityStatus::Pool::Types &cstatus, typename C::Score::Pool::Types &cscore
    ) override;
};

class MoveEnemy
    : public S::AMonoSystem<
          C::EntityStatus::Pool, C::Position::Pool, C::Velocity::Pool, C::Health::Pool> {
public:
    explicit MoveEnemy();
    ~MoveEnemy() override = default;

    int player_health = 0;

protected:
    void _innerOperate(
        typename C::EntityStatus::Pool::Types &cstatus,
        typename C::Position::Pool::Types &cposition, typename C::Velocity::Pool::Types &cvelocity,
        typename C::Health::Pool::Types &chealth
    ) override;
};

class DamageEnemy
    : public AMonoSystem<C::EntityStatus::Pool, C::Position::Pool, C::Size::Pool, C::Health::Pool> {
public:
    explicit DamageEnemy(AssetsLoader &assetsLoader, EntityManager &_eM);
    ~DamageEnemy() override = default;

    std::array<tower_info, TOWER_COUNT> towers;
    size_t money = 0;
    AssetsLoader &assetsLoader;
    EntityManager &_eM;
    size_t kills = 0;

protected:
    void _innerOperate(
        typename C::EntityStatus::Pool::Types &cstatus,
        typename C::Position::Pool::Types &cposition, typename C::Size::Pool::Types &csize,
        typename C::Health::Pool::Types &chealth
    ) override;
};

class KillProjectile
    : public AMonoSystem<
          C::EntityStatus::Pool, C::Position::Pool, C::Velocity::Pool, C::Dest::Pool> {
public:
    explicit KillProjectile();
    ~KillProjectile() override = default;

protected:
    void _innerOperate(
        typename C::EntityStatus::Pool::Types &crotation,
        typename C::Position::Pool::Types &cposition, typename C::Velocity::Pool::Types &cvelocity,
        typename C::Dest::Pool::Types & /*cdest*/
    ) override;
};

class DestroyEntitiesSystem : public S::AStatusMonoSystem<C::ChunkPos::Pool> {
public:
    explicit DestroyEntitiesSystem(EntityManager &entityManager);
    ~DestroyEntitiesSystem() override = default;

    EntityManager &entityManager;

protected:
    void _statusOperate(typename C::ChunkPos::Pool::Types &cchunkpos) override;
};

} // namespace ECS::S
