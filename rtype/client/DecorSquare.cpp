/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for DecorSquare entity declarations
*/

#include "DecorSquare.hpp"
#include "Square.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
DecorSquareRef::DecorSquareRef(
    C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *Position, C::SizeRef *Size,
    C::TypeRef *Type, C::SpriteRef *Sprite, C::TimerRef *Timer
):
    AEntityRef(status, cPos),
    _position(Position),
    _size(Size),
    _type(Type),
    _sprite(Sprite),
    _timer(Timer)
{
}

DecorSquareRef::~DecorSquareRef()
{
    delete _position;
    delete _size;
    delete _type;
    delete _sprite;
    delete _timer;
}

[[nodiscard]] C::PositionRef *E::DecorSquareRef::getPosition() const { return _position; }

void E::DecorSquareRef::setPosition(const C::PositionRef &position) { *_position = position; }

[[nodiscard]] C::SizeRef *E::DecorSquareRef::getSize() const { return _size; }

void E::DecorSquareRef::setSize(const C::SizeRef &size) { *_size = size; }

[[nodiscard]] C::TypeRef *E::DecorSquareRef::getType() const { return _type; }

void E::DecorSquareRef::setType(const C::TypeRef &type) { *_type = type; }

[[nodiscard]] C::SpriteRef *E::DecorSquareRef::getSprite() const { return _sprite; }

void E::DecorSquareRef::setSprite(const C::SpriteRef &sprite) { *_sprite = sprite; }

[[nodiscard]] C::TimerRef *E::DecorSquareRef::getTimer() const { return _timer; }

void E::DecorSquareRef::setTimer(const C::TimerRef &timer) { *_timer = timer; }

// ENTITY POOL
DecorSquarePool::DecorSquarePool():
    AEntityPool("DecorSquare", DecorSquareChunkSize)
{
}

std::unique_ptr<E::IEntityRef> DecorSquarePool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::DecorSquareRef> DecorSquarePool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::DecorSquareRef>(
        static_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
        static_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
        static_cast<C::PositionRef *>(_positionPool.getComponentRef(cPos)),
        static_cast<C::SizeRef *>(_sizePool.getComponentRef(cPos)),
        static_cast<C::TypeRef *>(_typePool.getComponentRef(cPos)),
        static_cast<C::SpriteRef *>(_spritePool.getComponentRef(cPos)),
        static_cast<C::TimerRef *>(_timerPool.getComponentRef(cPos))
    );
    return ptr;
}

std::vector<C::IComponentPool *> DecorSquarePool::getComponentPools()
{
    return {&_entityStatusPool, &_chunkPosPool, &_positionPool, &_sizePool,
            &_typePool,         &_spritePool,   &_timerPool};
}
}
