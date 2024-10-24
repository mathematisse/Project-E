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
    const ASpriteEntityRef &ent, C::TypeRef type

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
    return std::make_unique<E::DecorEntityRef>(getRawEntity(cPos));
}

E::DecorEntityRef DecorEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    return {ASpriteEntityPool::getRawEntity(cPos), EntityWithTypePool::getComponentRef(cPos)};
}

std::vector<C::IComponentPool *> DecorEntityPool::getComponentPools()
{
    auto base = AEntityPool::getComponentPools();
    auto stat = AStaticEntityPool::getComponentPools();
    auto sprite = ASpriteEntityPool::getComponentPools();
    auto sum = std::vector<C::IComponentPool *>(base.size() + stat.size() + sprite.size() + 1);
    std::copy(base.begin(), base.end(), sum.begin());
    std::copy(stat.begin(), stat.end(), sum.begin() + (long) base.size());
    std::copy(sprite.begin(), sprite.end(), sum.begin() + (long) base.size() + (long) stat.size());
    sum[base.size() + stat.size() + sprite.size()] = &EntityWithTypePool::getComponentPool();
    return sum;
}
/////////////////////////////////////////////////////////////////////////
// ENTITY REF
DecorAnimatedEntityRef::DecorAnimatedEntityRef(
    const AAnimatedEntityRef &ent, C::TypeRef type

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
    return std::make_unique<E::DecorAnimatedEntityRef>(getRawEntity(cPos));
}

E::DecorAnimatedEntityRef DecorAnimatedEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    return {AAnimatedEntityPool::getRawEntity(cPos), EntityWithTypePool::getComponentRef(cPos)};
}

std::vector<C::IComponentPool *> DecorAnimatedEntityPool::getComponentPools()
{
    auto base = AEntityPool::getComponentPools();
    auto stat = AStaticEntityPool::getComponentPools();
    auto sprite = AAnimatedEntityPool::getComponentPools();
    auto sum = std::vector<C::IComponentPool *>(base.size() + stat.size() + sprite.size() + 1);
    std::copy(base.begin(), base.end(), sum.begin());
    std::copy(stat.begin(), stat.end(), sum.begin() + (long) base.size());
    std::copy(sprite.begin(), sprite.end(), sum.begin() + (long) base.size() + (long) stat.size());
    sum[base.size() + stat.size() + sprite.size()] = &EntityWithTypePool::getComponentPool();
    return sum;
}
}
