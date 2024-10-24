/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for ASpriteEntity entity declarations
*/

#include "ASpriteEntity.hpp"
#include "spatial2d/Components.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
ASpriteEntityRef::ASpriteEntityRef(
    const AStaticEntityRef &ent, C::SpriteRef sprite, C::SizeRef size

):
    AEntityRef(ent),
    EntityWithPositionRef(ent),
    EntityWithRotationRef(ent),
    AStaticEntityRef(ent),
    EntityWithSpriteRef(sprite),
    EntityWithSizeRef(size)
{
}

// ENTITY POOL
ASpriteEntityPool::ASpriteEntityPool():
    AEntityPool("", 0)
{
}

std::unique_ptr<E::IEntityRef> ASpriteEntityPool::getEntity(Chunks::chunkPos_t cPos)
{
    return std::make_unique<E::ASpriteEntityRef>(getRawEntity(cPos));
}

E::ASpriteEntityRef ASpriteEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    return {
        AStaticEntityPool::getRawEntity(cPos), EntityWithSpritePool::getComponentRef(cPos),
        EntityWithSizePool::getComponentRef(cPos)
    };
}

std::vector<C::IComponentPool *> ASpriteEntityPool::getComponentPools()
{
    return {&EntityWithSpritePool::getComponentPool(), &EntityWithSizePool::getComponentPool()};
}
}
