/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for DecorEntity entity declarations
*/

#include "DecorEntities.hpp"
#include "spatial2d/AStaticEntity.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
DecorEntityRef::DecorEntityRef(
    const ASpriteEntityRef &ent, C::TypeRef *type

):
    AEntityRef(ent),
    EntityWithPositionRef(ent),
    EntityWithRotationRef(ent),
    AStaticEntityRef(ent),
    EntityWithSpriteRef(ent),
    EntityWithSizeRef(ent),
    ASpriteEntityRef(ent),
    EntityWithTypeRef(type)
{
}

// ENTITY POOL
DecorEntityPool::DecorEntityPool():
    AEntityPool("DecorEntity", DecorEntityChunkSize)
{
}

std::unique_ptr<E::IEntityRef> DecorEntityPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::DecorEntityRef> DecorEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::DecorEntityRef>(
        *ASpriteEntityPool::getRawEntity(cPos),
        EntityWithTypePool::getComponentRef(cPos)
    );
    return ptr;
}

std::vector<C::IComponentPool *> DecorEntityPool::getComponentPools()
{
    auto stat = AStaticEntityPool::getComponentPools();
    auto sprite = ASpriteEntityPool::getComponentPools();
    auto sum = std::vector<C::IComponentPool *>(stat.size() + sprite.size() + 1);
    std::copy(stat.begin(), stat.end(), sum.begin());
    std::copy(sprite.begin(), sprite.end(), sum.begin() + (long) stat.size());
    sum[stat.size() + sprite.size()] = &EntityWithTypePool::getComponentPool();
    return sum;
}
/////////////////////////////////////////////////////////////////////////
// ENTITY REF
DecorAnimatedEntityRef::DecorAnimatedEntityRef(
    const AAnimatedEntityRef &ent, C::TypeRef *type

):
    AEntityRef(ent),
    EntityWithPositionRef(ent),
    EntityWithRotationRef(ent),
    AStaticEntityRef(ent),
    EntityWithAnimatedSpriteRef(ent),
    EntityWithTimerRef(ent),
    EntityWithSizeRef(ent),
    AAnimatedEntityRef(ent),
    EntityWithTypeRef(type)
{
}

// ENTITY POOL
DecorAnimatedEntityPool::DecorAnimatedEntityPool():
    AEntityPool("DecorAnimatedEntity", DecorEntityChunkSize)
{
}

std::unique_ptr<E::IEntityRef> DecorAnimatedEntityPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::DecorAnimatedEntityRef> DecorAnimatedEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::DecorAnimatedEntityRef>(
        *AAnimatedEntityPool::getRawEntity(cPos),
        EntityWithTypePool::getComponentRef(cPos)
    );
    return ptr;
}

std::vector<C::IComponentPool *> DecorAnimatedEntityPool::getComponentPools()
{
    auto stat = AStaticEntityPool::getComponentPools();
    auto anim = AAnimatedEntityPool::getComponentPools();
    auto sum = std::vector<C::IComponentPool *>(stat.size() + anim.size() + 1);
    std::copy(stat.begin(), stat.end(), sum.begin());
    std::copy(anim.begin(), anim.end(), sum.begin() + (long) stat.size());
    sum[stat.size() + anim.size()] = &EntityWithTypePool::getComponentPool();
    return sum;
}
}
