/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for Entity entity declarations
*/

#pragma once

#include "DecorEntities.hpp"
#include "NetworkManager.hpp"
#include "core/Components.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "render/ASpriteEntity.hpp"
#include "spatial2d/ADynamicEntity.hpp"

enum class WeaponType {
    BULLET,
    BIG_SHOT,
};

DECLARE_COMPONENT(CanShoot, char, float, float);
DECLARE_COMPONENT(Health, size_t);
DECLARE_COMPONENT(IsShooting, char);
DECLARE_COMPONENT(Weapon, WeaponType);

namespace ECS::E {
const size_t EntityChunkSize = 64;

// ENTITY REF
class GameEntityRef : public ADynamicEntityRef,
                      public ASpriteEntityRef,
                      public EntityWithColorRef,
                      public EntityWithTypeRef,
                      public EntityWithNetworkIDRef,
                      public EntityWithCanShootRef,
                      public EntityWithIsShootingRef,
                      public EntityWithHealthRef,
                      public EntityWithWeaponRef {
public:
    GameEntityRef(
        const ADynamicEntityRef &dynEnt, const ASpriteEntityRef &spriteEnt, C::ColorRef color,
        C::TypeRef type, C::NetworkIDRef networkID, C::CanShootRef canShoot,
        C::IsShootingRef isShooting, C::HealthRef health, C::WeaponRef weapon
    );
};

// ENTITY POOL
class GameEntityPool : public ADynamicEntityPool,
                       public ASpriteEntityPool,
                       public EntityWithColorPool,
                       public EntityWithTypePool,
                       public EntityWithNetworkIDPool,
                       public EntityWithCanShootPool,
                       public EntityWithIsShootingPool,
                       public EntityWithHealthPool,
                       public EntityWithWeaponPool {
public:
    GameEntityPool();

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    E::GameEntityRef getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;
};

// ENTITY REF
class GameAnimatedEntityRef : public ADynamicEntityRef,
                              public AAnimatedEntityRef,
                              public EntityWithColorRef,
                              public EntityWithTypeRef,
                              public EntityWithNetworkIDRef,
                              public EntityWithCanShootRef,
                              public EntityWithIsShootingRef,
                              public EntityWithHealthRef,
                              public EntityWithWeaponRef {
public:
    GameAnimatedEntityRef(
        const ADynamicEntityRef &dynEnt, const AAnimatedEntityRef &spriteEnt, C::ColorRef color,
        C::TypeRef type, C::NetworkIDRef networkID, C::CanShootRef canShoot,
        C::IsShootingRef isShooting, C::HealthRef health, C::WeaponRef weapon
    );
};

// ENTITY POOL
class GameAnimatedEntityPool : public ADynamicEntityPool,
                               public AAnimatedEntityPool,
                               public EntityWithColorPool,
                               public EntityWithTypePool,
                               public EntityWithNetworkIDPool,
                               public EntityWithCanShootPool,
                               public EntityWithIsShootingPool,
                               public EntityWithHealthPool,
                               public EntityWithWeaponPool {
public:
    GameAnimatedEntityPool();

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    E::GameAnimatedEntityRef getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;
};
}
