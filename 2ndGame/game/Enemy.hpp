/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for Enemy entity declarations
*/

#pragma once

#include "Decor.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"

namespace ECS::C {
DECLARE_COMPONENT(Velocity, float, float);
DECLARE_COMPONENT(Health, size_t);
}

namespace ECS::E {
const size_t EnemyChunkSize = 1024;

// ENTITY REF
class EnemyRef : public AEntityRef {
public:
    EnemyRef(
        C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *position,
        C::SizeRef *size, C::SpriteRef *sprite, C::VelocityRef *velocity, C::HealthRef *health
    );
    ~EnemyRef() override;

    // Default rule of five
    EnemyRef(const EnemyRef &other) = default;
    EnemyRef &operator=(const EnemyRef &other) = default;
    EnemyRef(EnemyRef &&other) = default;
    EnemyRef &operator=(EnemyRef &&other) = default;

    // Accessors

    [[nodiscard]] C::PositionRef *getPosition() const;
    void setPosition(const C::PositionRef &position);

    [[nodiscard]] C::SizeRef *getSize() const;
    void setSize(const C::SizeRef &size);

    [[nodiscard]] C::SpriteRef *getSprite() const;
    void setSprite(const C::SpriteRef &sprite);

    [[nodiscard]] C::VelocityRef *getVelocity() const;
    void setVelocity(const C::VelocityRef &velocity);

    [[nodiscard]] C::HealthRef *getHealth() const;
    void setHealth(const C::HealthRef &health);

protected:
    C::PositionRef *_position;
    C::SizeRef *_size;
    C::SpriteRef *_sprite;
    C::VelocityRef *_velocity;
    C::HealthRef *_health;
};

// ENTITY POOL
class EnemyPool : public AEntityPool {
public:
    EnemyPool();
    ~EnemyPool() override = default;

    // default rule of five
    EnemyPool(const EnemyPool &other) = default;
    EnemyPool &operator=(const EnemyPool &other) = default;
    EnemyPool(EnemyPool &&other) = default;
    EnemyPool &operator=(EnemyPool &&other) = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::EnemyRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;

protected:
    C::PositionPool _positionPool;
    C::SizePool _sizePool;
    C::SpritePool _spritePool;
    C::VelocityPool _velocityPool;
    C::HealthPool _healthPool;
};
}
