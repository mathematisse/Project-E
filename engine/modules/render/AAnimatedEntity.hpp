/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for AAnimatedEntity declarations
*/

#pragma once

#include "core/Components.hpp"
#include "Components.hpp"
#include "spatial2d/AStaticEntity.hpp"

namespace ECS::E {

// ENTITY REF
class AAnimatedEntityRef : virtual public AStaticEntityRef, virtual public EntityWithAnimatedSpriteRef, virtual public EntityWithTimerRef, virtual public EntityWithSizeRef {
public:
    AAnimatedEntityRef(
        const AStaticEntityRef &ent, C::AnimatedSpriteRef *animatedSprite, C::TimerRef *timer, C::SizeRef *size
    );
    ~AAnimatedEntityRef() override = default;
};

// ENTITY POOL
class AAnimatedEntityPool : virtual public AStaticEntityPool, virtual public EntityWithAnimatedSpritePool, virtual public EntityWithTimerPool, virtual public EntityWithSizePool {
public:
    AAnimatedEntityPool();
    ~AAnimatedEntityPool() override = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::AAnimatedEntityRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;
};
}
