/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for Enemy entity declarations
*/

#include "Enemy.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
EnemyRef::EnemyRef(
    C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *Position, C::SizeRef *Size,
    C::SpriteRef *Sprite, C::VelocityRef *Velocity, C::HealthRef *Health
):
    AEntityRef(status, cPos),
    _position(Position),
    _size(Size),
    _sprite(Sprite),
    _velocity(Velocity),
    _health(Health)
{
}

EnemyRef::~EnemyRef()
{
    delete _position;
    delete _size;
    delete _sprite;
    delete _velocity;
    delete _health;
}

[[nodiscard]] C::PositionRef *E::EnemyRef::getPosition() const { return _position; }

void E::EnemyRef::setPosition(const C::PositionRef &position) { *_position = position; }

[[nodiscard]] C::SizeRef *E::EnemyRef::getSize() const { return _size; }

void E::EnemyRef::setSize(const C::SizeRef &size) { *_size = size; }

[[nodiscard]] C::SpriteRef *E::EnemyRef::getSprite() const { return _sprite; }

void E::EnemyRef::setSprite(const C::SpriteRef &sprite) { *_sprite = sprite; }

[[nodiscard]] C::VelocityRef *E::EnemyRef::getVelocity() const { return _velocity; }

void E::EnemyRef::setVelocity(const C::VelocityRef &velocity) { *_velocity = velocity; }

[[nodiscard]] C::HealthRef *E::EnemyRef::getHealth() const { return _health; }

void E::EnemyRef::setHealth(const C::HealthRef &health) { *_health = health; }

// ENTITY POOL
EnemyPool::EnemyPool():
    AEntityPool("Enemy", EnemyChunkSize)
{
}

std::unique_ptr<E::IEntityRef> EnemyPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::EnemyRef> EnemyPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::EnemyRef>(
        static_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
        static_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
        static_cast<C::PositionRef *>(_positionPool.getComponentRef(cPos)),
        static_cast<C::SizeRef *>(_sizePool.getComponentRef(cPos)),
        static_cast<C::SpriteRef *>(_spritePool.getComponentRef(cPos)),
        static_cast<C::VelocityRef *>(_velocityPool.getComponentRef(cPos)),
        static_cast<C::HealthRef *>(_healthPool.getComponentRef(cPos))
    );
    return ptr;
}

std::vector<C::IComponentPool *> EnemyPool::getComponentPools()
{
    return {&_entityStatusPool, &_chunkPosPool, &_positionPool, &_sizePool,
            &_spritePool,       &_velocityPool, &_healthPool};
}
}
