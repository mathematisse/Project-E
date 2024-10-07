/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for Square entity declarations
*/

#include "Square.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
SquareRef::SquareRef(
    C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *Position, C::VelocityRef *Velocity,
    C::ColorRef *Color, C::SizeRef *Size, C::TypeRef *Type, C::CanShootRef *CanShoot, C::SpriteRef *Sprite
):
    AEntityRef(status, cPos),
    _position(Position),
    _velocity(Velocity),
    _color(Color),
    _size(Size),
    _type(Type),
    _canShoot(CanShoot),
    _sprite(Sprite)
{
}

SquareRef::~SquareRef()
{
    delete _position;
    delete _velocity;
    delete _color;
    delete _size;
    delete _type;
    delete _canShoot;
    delete _sprite;
}

[[nodiscard]] C::PositionRef *E::SquareRef::getPosition() const { return _position; }

void E::SquareRef::setPosition(const C::PositionRef &position) { *_position = position; }

[[nodiscard]] C::VelocityRef *E::SquareRef::getVelocity() const { return _velocity; }

void E::SquareRef::setVelocity(const C::VelocityRef &velocity) { *_velocity = velocity; }

[[nodiscard]] C::ColorRef *E::SquareRef::getColor() const { return _color; }

void E::SquareRef::setColor(const C::ColorRef &color) { *_color = color; }

[[nodiscard]] C::SizeRef *E::SquareRef::getSize() const { return _size; }

void E::SquareRef::setSize(const C::SizeRef &size) { *_size = size; }

[[nodiscard]] C::TypeRef *E::SquareRef::getType() const { return _type; }

void E::SquareRef::setType(const C::TypeRef &type) { *_type = type; }

[[nodiscard]] C::CanShootRef *E::SquareRef::getCanShoot() const { return _canShoot; }

void E::SquareRef::setCanShoot(const C::CanShootRef &canShoot) { *_canShoot = canShoot; }

[[nodiscard]] C::SpriteRef *E::SquareRef::getSprite() const { return _sprite; }

void E::SquareRef::setSprite(const C::SpriteRef &sprite) { *_sprite = sprite; }

// ENTITY POOL
SquarePool::SquarePool():
    AEntityPool("Square", SquareChunkSize)
{
}

std::unique_ptr<E::IEntityRef> SquarePool::getEntity(Chunks::chunkPos_t cPos) { return getRawEntity(cPos); }

std::unique_ptr<E::SquareRef> SquarePool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::SquareRef>(
        static_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
        static_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
        static_cast<C::PositionRef *>(_positionPool.getComponentRef(cPos)),
        static_cast<C::VelocityRef *>(_velocityPool.getComponentRef(cPos)),
        static_cast<C::ColorRef *>(_colorPool.getComponentRef(cPos)),
        static_cast<C::SizeRef *>(_sizePool.getComponentRef(cPos)),
        static_cast<C::TypeRef *>(_typePool.getComponentRef(cPos)),
        static_cast<C::CanShootRef *>(_canShootPool.getComponentRef(cPos)),
        static_cast<C::SpriteRef *>(_spritePool.getComponentRef(cPos))
    );
    return ptr;
}

std::vector<C::IComponentPool *> SquarePool::getComponentPools()
{
    return {&_entityStatusPool, &_chunkPosPool, &_positionPool, &_velocityPool, &_colorPool,
            &_sizePool,         &_typePool,     &_canShootPool, &_spritePool};
}
}
