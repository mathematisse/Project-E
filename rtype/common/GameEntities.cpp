/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for Entity entity declarations
*/

#include "GameEntities.hpp"
#include "core/Components.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
GameEntityRef::GameEntityRef(
    const ADynamicEntityRef &dynEnt, const ASpriteEntityRef &spriteEnt, C::ColorRef *color,
    C::TypeRef *type, C::NetworkIDRef *networkID, C::CanShootRef *canShoot,
    C::IsShootingRef *isShooting, C::HealthRef *health, C::WeaponRef *weapon
):
    AEntityRef(dynEnt),
    EntityWithPositionRef(dynEnt),
    EntityWithRotationRef(dynEnt),
    AStaticEntityRef(dynEnt),
    EntityWithVelocityRef(dynEnt),
    EntityWithAccelerationRef(dynEnt),
    EntityWithSpriteRef(spriteEnt),
    EntityWithSizeRef(spriteEnt),
    ADynamicEntityRef(dynEnt),
    ASpriteEntityRef(spriteEnt),
    EntityWithColorRef(color),
    EntityWithTypeRef(type),
    EntityWithNetworkIDRef(networkID),
    EntityWithCanShootRef(canShoot),
    EntityWithIsShootingRef(isShooting),
    EntityWithHealthRef(health),
    EntityWithWeaponRef(weapon)
{
}

// ENTITY POOL
GameEntityPool::GameEntityPool():
    AEntityPool("GameEntity", EntityChunkSize)
{
}

std::unique_ptr<E::IEntityRef> GameEntityPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::GameEntityRef> GameEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::GameEntityRef>(
        *ADynamicEntityPool::getRawEntity(cPos), *ASpriteEntityPool::getRawEntity(cPos),
        EntityWithColorPool::getComponentRef(cPos), EntityWithTypePool::getComponentRef(cPos),
        EntityWithNetworkIDPool::getComponentRef(cPos),
        EntityWithCanShootPool::getComponentRef(cPos),
        EntityWithIsShootingPool::getComponentRef(cPos),
        EntityWithHealthPool::getComponentRef(cPos), EntityWithWeaponPool::getComponentRef(cPos)
    );
    return ptr;
}

std::vector<C::IComponentPool *> GameEntityPool::getComponentPools()
{
    auto stat = AStaticEntityPool::getComponentPools();
    auto dyn = ADynamicEntityPool::getComponentPools();
    auto sprite = ASpriteEntityPool::getComponentPools();
    std::vector<C::IComponentPool *> local = {
        &EntityWithColorPool::getComponentPool(),      &EntityWithTypePool::getComponentPool(),
        &EntityWithNetworkIDPool::getComponentPool(),  &EntityWithCanShootPool::getComponentPool(),
        &EntityWithIsShootingPool::getComponentPool(), &EntityWithHealthPool::getComponentPool(),
        &EntityWithWeaponPool::getComponentPool()
    };
    auto sum =
        std::vector<C::IComponentPool *>(stat.size() + dyn.size() + sprite.size() + local.size());
    std::copy(stat.begin(), stat.end(), sum.begin());
    std::copy(dyn.begin(), dyn.end(), sum.begin() + (long) stat.size());
    std::copy(sprite.begin(), sprite.end(), sum.begin() + (long) stat.size() + (long) dyn.size());
    std::copy(
        local.begin(), local.end(),
        sum.begin() + (long) stat.size() + (long) dyn.size() + (long) sprite.size()
    );
    return sum;
}

///////////////////////////////////////////////////////////////////////////

// ENTITY REF
GameAnimatedEntityRef::GameAnimatedEntityRef(
    const ADynamicEntityRef &dynEnt, const AAnimatedEntityRef &spriteEnt, C::ColorRef *color,
    C::TypeRef *type, C::NetworkIDRef *networkID, C::CanShootRef *canShoot,
    C::IsShootingRef *isShooting, C::HealthRef *health, C::WeaponRef *weapon
):
    AEntityRef(dynEnt),
    EntityWithPositionRef(dynEnt),
    EntityWithRotationRef(dynEnt),
    AStaticEntityRef(dynEnt),
    EntityWithVelocityRef(dynEnt),
    EntityWithAccelerationRef(dynEnt),
    EntityWithAnimatedSpriteRef(spriteEnt),
    EntityWithTimerRef(spriteEnt),
    EntityWithSizeRef(spriteEnt),
    ADynamicEntityRef(dynEnt),
    AAnimatedEntityRef(spriteEnt),
    EntityWithColorRef(color),
    EntityWithTypeRef(type),
    EntityWithNetworkIDRef(networkID),
    EntityWithCanShootRef(canShoot),
    EntityWithIsShootingRef(isShooting),
    EntityWithHealthRef(health),
    EntityWithWeaponRef(weapon)
{
}

// ENTITY POOL
GameAnimatedEntityPool::GameAnimatedEntityPool():
    AEntityPool("GameAnimatedEntity", EntityChunkSize)
{
}

std::unique_ptr<E::IEntityRef> GameAnimatedEntityPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::GameAnimatedEntityRef>
GameAnimatedEntityPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::GameAnimatedEntityRef>(
        *ADynamicEntityPool::getRawEntity(cPos), *AAnimatedEntityPool::getRawEntity(cPos),
        EntityWithColorPool::getComponentRef(cPos), EntityWithTypePool::getComponentRef(cPos),
        EntityWithNetworkIDPool::getComponentRef(cPos),
        EntityWithCanShootPool::getComponentRef(cPos),
        EntityWithIsShootingPool::getComponentRef(cPos),
        EntityWithHealthPool::getComponentRef(cPos), EntityWithWeaponPool::getComponentRef(cPos)
    );
    return ptr;
}

std::vector<C::IComponentPool *> GameAnimatedEntityPool::getComponentPools()
{
    auto stat = AStaticEntityPool::getComponentPools();
    auto dyn = ADynamicEntityPool::getComponentPools();
    auto sprite = AAnimatedEntityPool::getComponentPools();
    std::vector<C::IComponentPool *> local = {
        &EntityWithColorPool::getComponentPool(),      &EntityWithTypePool::getComponentPool(),
        &EntityWithNetworkIDPool::getComponentPool(),  &EntityWithCanShootPool::getComponentPool(),
        &EntityWithIsShootingPool::getComponentPool(), &EntityWithHealthPool::getComponentPool(),
        &EntityWithWeaponPool::getComponentPool()
    };
    auto sum =
        std::vector<C::IComponentPool *>(stat.size() + dyn.size() + sprite.size() + local.size());
    std::copy(stat.begin(), stat.end(), sum.begin());
    std::copy(dyn.begin(), dyn.end(), sum.begin() + (long) stat.size());
    std::copy(sprite.begin(), sprite.end(), sum.begin() + (long) stat.size() + (long) dyn.size());
    std::copy(
        local.begin(), local.end(),
        sum.begin() + (long) stat.size() + (long) dyn.size() + (long) sprite.size()
    );
    return sum;
}

}
