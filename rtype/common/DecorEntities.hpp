/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for DecorEntity entity declarations
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "render/AAnimatedEntity.hpp"
#include "render/ASpriteEntity.hpp"

enum class GameEntityType {
    LPLAYER,
    PLAYER,
    ENEMY,
    BULLET,
    BULLET_ENNEMY,
    WALL,
    POWERUP,
    BACKGROUND,
    ENGINE
};

DECLARE_COMPONENT(Type, GameEntityType);

namespace ECS::E {
const size_t DecorEntityChunkSize = 64;

// ENTITY REF
class DecorEntityRef : public ASpriteEntityRef, public EntityWithTypeRef {
public:
    DecorEntityRef(
        const ASpriteEntityRef &ent, C::TypeRef *type
    );
    ~DecorEntityRef() override = default;
};

// ENTITY POOL
class DecorEntityPool : public ASpriteEntityPool, public EntityWithTypePool {
public:
    DecorEntityPool();
    ~DecorEntityPool() override = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::DecorEntityRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;
};

// ENTITY REF
class DecorAnimatedEntityRef : public AAnimatedEntityRef, public EntityWithTypeRef {
public:
    DecorAnimatedEntityRef(
        const AAnimatedEntityRef &ent, C::TypeRef *type
    );
    ~DecorAnimatedEntityRef() override = default;
};

// ENTITY POOL
class DecorAnimatedEntityPool : public AAnimatedEntityPool, public EntityWithTypePool {
public:
    DecorAnimatedEntityPool();
    ~DecorAnimatedEntityPool() override = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::DecorAnimatedEntityRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;
};
}
