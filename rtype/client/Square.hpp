/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for Square entity declarations
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"

enum class SquareType {
    PLAYER,
    ENEMY,
    BULLET,
    WALL,
    POWERUP,
    BACKGROUND
};

namespace ECS::C {
DECLARE_COMPONENT(Position, float, float);
DECLARE_COMPONENT(Velocity, float, float, float); // 3rd param is the speed
DECLARE_COMPONENT(Color, unsigned char, unsigned char, unsigned char, unsigned char);
DECLARE_COMPONENT(Size, float, float, float); // 3rd param is the rotation
DECLARE_COMPONENT(Type, SquareType);
DECLARE_COMPONENT(CanShoot, char, float, float)
DECLARE_COMPONENT(Sprite, size_t)
}

namespace ECS::E {
const size_t SquareChunkSize = 1024;

// ENTITY REF
class SquareRef : public AEntityRef {
public:
    SquareRef(
        C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *position, C::VelocityRef *velocity,
        C::ColorRef *color, C::SizeRef *size, C::TypeRef *type, C::CanShootRef *canShoot, C::SpriteRef *sprite
    );
    ~SquareRef() override;

    // Default rule of five
    SquareRef(const SquareRef &other) = default;
    SquareRef &operator=(const SquareRef &other) = default;
    SquareRef(SquareRef &&other) = default;
    SquareRef &operator=(SquareRef &&other) = default;

    // Accessors

    [[nodiscard]] C::PositionRef *getPosition() const;
    void setPosition(const C::PositionRef &position);

    [[nodiscard]] C::VelocityRef *getVelocity() const;
    void setVelocity(const C::VelocityRef &velocity);

    [[nodiscard]] C::ColorRef *getColor() const;
    void setColor(const C::ColorRef &color);

    [[nodiscard]] C::SizeRef *getSize() const;
    void setSize(const C::SizeRef &size);

    [[nodiscard]] C::TypeRef *getType() const;
    void setType(const C::TypeRef &type);

    [[nodiscard]] C::CanShootRef *getCanShoot() const;
    void setCanShoot(const C::CanShootRef &canShoot);

    [[nodiscard]] C::SpriteRef *getSprite() const;
    void setSprite(const C::SpriteRef &sprite);

protected:
    C::PositionRef *_position;
    C::VelocityRef *_velocity;
    C::ColorRef *_color;
    C::SizeRef *_size;
    C::TypeRef *_type;
    C::CanShootRef *_canShoot;
    C::SpriteRef *_sprite;
};

// ENTITY POOL
class SquarePool : public AEntityPool {
public:
    SquarePool();
    ~SquarePool() override = default;

    // default rule of five
    SquarePool(const SquarePool &other) = default;
    SquarePool &operator=(const SquarePool &other) = default;
    SquarePool(SquarePool &&other) = default;
    SquarePool &operator=(SquarePool &&other) = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::SquareRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;

protected:
    C::PositionPool _positionPool;
    C::VelocityPool _velocityPool;
    C::ColorPool _colorPool;
    C::SizePool _sizePool;
    C::TypePool _typePool;
    C::CanShootPool _canShootPool;
    C::SpritePool _spritePool;
};
}
