/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for Decor entity declarations
*/

#include "Decor.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
DecorRef::DecorRef(
    C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *Position, C::SizeRef *Size,
    C::SpriteRef *Sprite
):
    AEntityRef(status, cPos),
    _position(Position),
    _size(Size),
    _sprite(Sprite)
{
}

DecorRef::~DecorRef()
{
    delete _position;
    delete _size;
    delete _sprite;
}

[[nodiscard]] C::PositionRef *E::DecorRef::getPosition() const { return _position; }

void E::DecorRef::setPosition(const C::PositionRef &position) { *_position = position; }

[[nodiscard]] C::SizeRef *E::DecorRef::getSize() const { return _size; }

void E::DecorRef::setSize(const C::SizeRef &size) { *_size = size; }

[[nodiscard]] C::SpriteRef *E::DecorRef::getSprite() const { return _sprite; }

void E::DecorRef::setSprite(const C::SpriteRef &sprite) { *_sprite = sprite; }

// ENTITY POOL
DecorPool::DecorPool():
    AEntityPool("Decor", DecorChunkSize)
{
}

std::unique_ptr<E::IEntityRef> DecorPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::DecorRef> DecorPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::DecorRef>(
        static_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
        static_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
        static_cast<C::PositionRef *>(_positionPool.getComponentRef(cPos)),
        static_cast<C::SizeRef *>(_sizePool.getComponentRef(cPos)),
        static_cast<C::SpriteRef *>(_spritePool.getComponentRef(cPos))
    );
    return ptr;
}

std::vector<C::IComponentPool *> DecorPool::getComponentPools()
{
    return {&_entityStatusPool, &_chunkPosPool, &_positionPool, &_sizePool, &_spritePool};
}
}
