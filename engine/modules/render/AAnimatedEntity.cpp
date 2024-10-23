/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for AAnimatedEntity entity declarations
*/

#include "AAnimatedEntity.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
AAnimatedEntityRef::AAnimatedEntityRef(
    const AStaticEntityRef &ent, C::AnimatedSpriteRef *animatedSprite, C::TimerRef *timer,
    C::SizeRef *size

):
    AEntityRef(ent),
    EntityWithPositionRef(ent),
    EntityWithRotationRef(ent),
    AStaticEntityRef(ent),
    EntityWithAnimatedSpriteRef(animatedSprite),
    EntityWithTimerRef(timer),
    EntityWithSizeRef(size)
{
}

// ENTITY POOL
AAnimatedEntityPool::AAnimatedEntityPool():
    AEntityPool("", 0)
{
}

std::unique_ptr<E::IEntityRef> AAnimatedEntityPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::AAnimatedEntityRef> AAnimatedEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::AAnimatedEntityRef>(
        *AStaticEntityPool::getRawEntity(cPos), EntityWithAnimatedSpritePool::getComponentRef(cPos),
        EntityWithTimerPool::getComponentRef(cPos), EntityWithSizePool::getComponentRef(cPos)
    );
    return ptr;
}

std::vector<C::IComponentPool *> AAnimatedEntityPool::getComponentPools()
{
    return {
        &EntityWithAnimatedSpritePool::getComponentPool(), &EntityWithTimerPool::getComponentPool()
    };
}
}
