/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for Projectile entity declarations
*/

#include "Projectile.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
ProjectileRef::ProjectileRef(
    C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *Position, C::SizeRef *Size,
    C::SpriteRef *Sprite, C::VelocityRef *Velocity, C::DestRef *Dest
):
    AEntityRef(status, cPos),
    _postion(Position),
    _size(Size),
    _sprite(Sprite),
    _velocity(Velocity),
    _dest(Dest)
{
}

ProjectileRef::~ProjectileRef()
{
    delete _postion;
    delete _size;
    delete _sprite;
    delete _velocity;
    delete _dest;
}

[[nodiscard]] C::PositionRef *E::ProjectileRef::getPosition() const { return _postion; }

void E::ProjectileRef::setPosition(const C::PositionRef &postion) { *_postion = postion; }

[[nodiscard]] C::SizeRef *E::ProjectileRef::getSize() const { return _size; }

void E::ProjectileRef::setSize(const C::SizeRef &size) { *_size = size; }

[[nodiscard]] C::SpriteRef *E::ProjectileRef::getSprite() const { return _sprite; }

void E::ProjectileRef::setSprite(const C::SpriteRef &sprite) { *_sprite = sprite; }

[[nodiscard]] C::VelocityRef *E::ProjectileRef::getVelocity() const { return _velocity; }

void E::ProjectileRef::setVelocity(const C::VelocityRef &velocity) { *_velocity = velocity; }

[[nodiscard]] C::DestRef *E::ProjectileRef::getDest() const { return _dest; }

void E::ProjectileRef::setDest(const C::DestRef &dest) { *_dest = dest; }

// ENTITY POOL
ProjectilePool::ProjectilePool():
    AEntityPool("Projectile", ProjectileChunkSize)
{
}

std::unique_ptr<E::IEntityRef> ProjectilePool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::ProjectileRef> ProjectilePool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::ProjectileRef>(
        static_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
        static_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
        static_cast<C::PositionRef *>(_postionPool.getComponentRef(cPos)),
        static_cast<C::SizeRef *>(_sizePool.getComponentRef(cPos)),
        static_cast<C::SpriteRef *>(_spritePool.getComponentRef(cPos)),
        static_cast<C::VelocityRef *>(_velocityPool.getComponentRef(cPos)),
        static_cast<C::DestRef *>(_destPool.getComponentRef(cPos))
    );
    return ptr;
}

std::vector<C::IComponentPool *> ProjectilePool::getComponentPools()
{
    return {&_entityStatusPool, &_chunkPosPool, &_postionPool, &_sizePool,
            &_spritePool,       &_velocityPool, &_destPool};
}
}
