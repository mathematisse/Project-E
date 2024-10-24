/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for ASpriteEntity declarations
*/

#pragma once

#include "Components.hpp"
#include "spatial2d/AStaticEntity.hpp"

namespace ECS::E {

// ENTITY REF
class ASpriteEntityRef : virtual public AStaticEntityRef,
                         virtual public EntityWithSpriteRef,
                         virtual public EntityWithSizeRef {
public:
    ASpriteEntityRef(const AStaticEntityRef &ent, C::SpriteRef sprite, C::SizeRef size);
    ~ASpriteEntityRef() override = default;
};

// ENTITY POOL
class ASpriteEntityPool : virtual public AStaticEntityPool,
                          virtual public EntityWithSpritePool,
                          virtual public EntityWithSizePool {
public:
    ASpriteEntityPool();
    ~ASpriteEntityPool() override = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    E::ASpriteEntityRef getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;
};
}
