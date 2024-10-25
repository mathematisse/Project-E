/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for Projectile entity declarations
*/

#pragma once

#include "Decor.hpp"
#include "Enemy.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"
#include <raylib.h>

namespace ECS::C {
DECLARE_COMPONENT(Dest, Vector2);
}

namespace ECS::E {
const size_t ProjectileChunkSize = 1024;

// ENTITY REF
class ProjectileRef : public AEntityRef {
public:
    ProjectileRef(
        C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *postion, C::SizeRef *size,
        C::SpriteRef *sprite, C::VelocityRef *velocity, C::DestRef *dest
    );
    ~ProjectileRef() override;

    // Default rule of five
    ProjectileRef(const ProjectileRef &other) = default;
    ProjectileRef &operator=(const ProjectileRef &other) = default;
    ProjectileRef(ProjectileRef &&other) = default;
    ProjectileRef &operator=(ProjectileRef &&other) = default;

    // Accessors

    [[nodiscard]] C::PositionRef *getPosition() const;
    void setPosition(const C::PositionRef &postion);

    [[nodiscard]] C::SizeRef *getSize() const;
    void setSize(const C::SizeRef &size);

    [[nodiscard]] C::SpriteRef *getSprite() const;
    void setSprite(const C::SpriteRef &sprite);

    [[nodiscard]] C::VelocityRef *getVelocity() const;
    void setVelocity(const C::VelocityRef &velocity);

    [[nodiscard]] C::DestRef *getDest() const;
    void setDest(const C::DestRef &Dest);

protected:
    C::PositionRef *_postion;
    C::SizeRef *_size;
    C::SpriteRef *_sprite;
    C::VelocityRef *_velocity;
    C::DestRef *_dest;
};

// ENTITY POOL
class ProjectilePool : public AEntityPool {
public:
    ProjectilePool();
    ~ProjectilePool() override = default;

    // default rule of five
    ProjectilePool(const ProjectilePool &other) = default;
    ProjectilePool &operator=(const ProjectilePool &other) = default;
    ProjectilePool(ProjectilePool &&other) = default;
    ProjectilePool &operator=(ProjectilePool &&other) = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::ProjectileRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;

protected:
    C::PositionPool _postionPool;
    C::SizePool _sizePool;
    C::SpritePool _spritePool;
    C::VelocityPool _velocityPool;
    C::DestPool _destPool;
};
}
