/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for Tower entity declarations
*/

#include "Tower.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
TowerRef::TowerRef(
    C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *Position, C::SizeRef *Size,
    C::SpriteRef *Sprite, C::TypeRef *Type, C::LevelRef *Level, C::IDRef *ID
):
    AEntityRef(status, cPos),
    _position(Position),
    _size(Size),
    _sprite(Sprite),
    _type(Type),
    _level(Level),
    _id(ID)
{
}

TowerRef::~TowerRef()
{
    delete _position;
    delete _size;
    delete _sprite;
    delete _type;
    delete _level;
    delete _id;
}

[[nodiscard]] C::PositionRef *E::TowerRef::getPosition() const { return _position; }

void E::TowerRef::setPosition(const C::PositionRef &position) { *_position = position; }

[[nodiscard]] C::SizeRef *E::TowerRef::getSize() const { return _size; }

void E::TowerRef::setSize(const C::SizeRef &size) { *_size = size; }

[[nodiscard]] C::SpriteRef *E::TowerRef::getSprite() const { return _sprite; }

void E::TowerRef::setSprite(const C::SpriteRef &sprite) { *_sprite = sprite; }

[[nodiscard]] C::TypeRef *E::TowerRef::getType() const { return _type; }

void E::TowerRef::setType(const C::TypeRef &type) { *_type = type; }

[[nodiscard]] C::LevelRef *E::TowerRef::getLevel() const { return _level; }

void E::TowerRef::setLevel(const C::LevelRef &level) { *_level = level; }

[[nodiscard]] C::IDRef *E::TowerRef::getID() const { return _id; }

void E::TowerRef::setID(const C::IDRef &id) { *_id = id; }

// ENTITY POOL
TowerPool::TowerPool():
    AEntityPool("Tower", TowerChunkSize)
{
}

std::unique_ptr<E::IEntityRef> TowerPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::TowerRef> TowerPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::TowerRef>(
        static_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
        static_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
        static_cast<C::PositionRef *>(_positionPool.getComponentRef(cPos)),
        static_cast<C::SizeRef *>(_sizePool.getComponentRef(cPos)),
        static_cast<C::SpriteRef *>(_spritePool.getComponentRef(cPos)),
        static_cast<C::TypeRef *>(_typePool.getComponentRef(cPos)),
        static_cast<C::LevelRef *>(_levelPool.getComponentRef(cPos)),
        static_cast<C::IDRef *>(_idPool.getComponentRef(cPos))
    );
    return ptr;
}

std::vector<C::IComponentPool *> TowerPool::getComponentPools()
{
    return {&_entityStatusPool, &_chunkPosPool, &_positionPool, &_sizePool,
            &_spritePool,       &_typePool,     &_levelPool,    &_idPool};
}
}
