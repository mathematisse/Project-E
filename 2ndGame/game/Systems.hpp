/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Decor.hpp"
#include "Tower.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "AssetsLoader.hpp"
#include "raylib.h"
#include <tuple>

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

class DrawRotationProjectileSystem : public S::AMonoSystem<
                                         C::EntityStatusPool, C::PositionPool, C::SizePool,
                                         C::SpritePool, C::VelocityPool, C::DestPool> {
public:
    explicit DrawRotationProjectileSystem(AssetsLoader &assetsLoader);
    ~DrawRotationProjectileSystem() override = default;

    DrawRotationProjectileSystem(const DrawRotationProjectileSystem &other) = default;
    DrawRotationProjectileSystem(DrawRotationProjectileSystem &&other) = default;
    DrawRotationProjectileSystem &operator=(const DrawRotationProjectileSystem &other) = default;
    DrawRotationProjectileSystem &operator=(DrawRotationProjectileSystem &&other) = default;

    AssetsLoader &assetsLoader;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::SizePool::Types &csize, typename C::SpritePool::Types &csprite,
        typename C::VelocityPool::Types &cvelocity, typename C::DestPool::Types &cdest
    ) override;
};

class ApplyVelocitySystem : public S::AStatusMonoSystem<C::PositionPool, C::VelocityPool> {
public:
    explicit ApplyVelocitySystem();
    ~ApplyVelocitySystem() override = default;

    ApplyVelocitySystem(const ApplyVelocitySystem &other) = default;
    ApplyVelocitySystem(ApplyVelocitySystem &&other) = default;
    ApplyVelocitySystem &operator=(const ApplyVelocitySystem &other) = default;
    ApplyVelocitySystem &operator=(ApplyVelocitySystem &&other) = default;

    float deltaTime = 0.0f;

protected:
    void _statusOperate(
        typename C::PositionPool::Types &cposition, typename C::VelocityPool::Types &cvelocity
    ) override;
};

struct tower {
    size_t id;
    size_t level;
    towerType type;
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
    tower selectedTower = {0, 0, NONE};

protected:
    void _innerOperate(
        typename C::PositionPool::Types &cposition, typename C::SizePool::Types &csize,
        typename C::TypePool::Types &ctype, typename C::LevelPool::Types &clevel,
        typename C::IDPool::Types &cid
    ) override;
};

class ChangeTowerSprite
    : public S::AMonoSystem<C::SpritePool, C::TypePool, C::LevelPool, C::IDPool> {
public:
    explicit ChangeTowerSprite(AssetsLoader &assetsLoader);
    ~ChangeTowerSprite() override = default;

    ChangeTowerSprite(const ChangeTowerSprite &other) = default;
    ChangeTowerSprite(ChangeTowerSprite &&other) = default;
    ChangeTowerSprite &operator=(const ChangeTowerSprite &other) = default;
    ChangeTowerSprite &operator=(ChangeTowerSprite &&other) = default;

    AssetsLoader &assetsLoader;

protected:
    void _innerOperate(
        typename C::SpritePool::Types &csprite, typename C::TypePool::Types &ctype,
        typename C::LevelPool::Types &clevel, typename C::IDPool::Types &cid
    ) override;
};

class SpawnEnemy : public S::AMonoSystem<C::EntityStatusPool, C::ScorePool> {
public:
    explicit SpawnEnemy(AssetsLoader &AssetsLoader, EntityManager &_eM);
    ~SpawnEnemy() override = default;

    SpawnEnemy(const SpawnEnemy &other) = default;
    SpawnEnemy(SpawnEnemy &&other) = default;
    SpawnEnemy &operator=(const SpawnEnemy &other) = default;
    SpawnEnemy &operator=(SpawnEnemy &&other) = default;

    AssetsLoader &assetsLoader;
    EntityManager &_eM;
    float delay = 0;
    size_t kills = 0;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::ScorePool::Types &cscore
    ) override;
};

class MoveEnemy
    : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::VelocityPool, C::HealthPool> {
public:
    explicit MoveEnemy();
    ~MoveEnemy() override = default;

    MoveEnemy(const MoveEnemy &other) = default;
    MoveEnemy(MoveEnemy &&other) = default;
    MoveEnemy &operator=(const MoveEnemy &other) = default;
    MoveEnemy &operator=(MoveEnemy &&other) = default;

    int player_health = 0;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::VelocityPool::Types &cvelocity, typename C::HealthPool::Types &chealth
    ) override;
};

class DamageEnemy
    : public AMonoSystem<C::EntityStatusPool, C::PositionPool, C::SizePool, C::HealthPool> {
public:
    explicit DamageEnemy(AssetsLoader &assetsLoader, EntityManager &_eM);
    ~DamageEnemy() override = default;

    DamageEnemy(const DamageEnemy &other) = default;
    DamageEnemy(DamageEnemy &&other) = default;
    DamageEnemy &operator=(const DamageEnemy &other) = default;
    DamageEnemy &operator=(DamageEnemy &&other) = default;

    std::vector<tower_info> towers;
    size_t money = 0;
    AssetsLoader &assetsLoader;
    EntityManager &_eM;
    size_t kills = 0;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::SizePool::Types &csize, typename C::HealthPool::Types &chealth
    ) override;
};

class KillProjectile
    : public AMonoSystem<C::EntityStatusPool, C::PositionPool, C::VelocityPool, C::DestPool> {
public:
    explicit KillProjectile();
    ~KillProjectile() override = default;

    KillProjectile(const KillProjectile &other) = default;
    KillProjectile(KillProjectile &&other) = default;
    KillProjectile &operator=(const KillProjectile &other) = default;
    KillProjectile &operator=(KillProjectile &&other) = default;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition,
        typename C::VelocityPool::Types &cvelocity, typename C::DestPool::Types &cdest
    ) override;
};

} // namespace ECS::S
