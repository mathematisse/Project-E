/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for DecorSquare entity declarations
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"

enum class SquareType {
    PLAYER,
    ENEMY,
    BULLET,
    BULLET_ENNEMY,
    WALL,
    POWERUP,
    BACKGROUND,
    ENGINE
};

namespace ECS::C {
DECLARE_COMPONENT(Position, float, float);
DECLARE_COMPONENT(Size, float, float, float); // 3rd param is the rotation
DECLARE_COMPONENT(Type, SquareType);
DECLARE_COMPONENT(Sprite, size_t, char, float, float, float, float, float);
DECLARE_COMPONENT(Timer, float, float);
}

namespace ECS::E {
const size_t DecorSquareChunkSize = 64;

// ENTITY REF
class DecorSquareRef : public AEntityRef {
public:
    DecorSquareRef(
        C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *position, C::SizeRef *size,
        C::TypeRef *type, C::SpriteRef *sprite, C::TimerRef *timer
    );
    ~DecorSquareRef() override;

    // Default rule of five
    DecorSquareRef(const DecorSquareRef &other) = default;
    DecorSquareRef &operator=(const DecorSquareRef &other) = default;
    DecorSquareRef(DecorSquareRef &&other) = default;
    DecorSquareRef &operator=(DecorSquareRef &&other) = default;

    // Accessors

    [[nodiscard]] C::PositionRef *getPosition() const;
    void setPosition(const C::PositionRef &position);

    [[nodiscard]] C::SizeRef *getSize() const;
    void setSize(const C::SizeRef &size);

    [[nodiscard]] C::TypeRef *getType() const;
    void setType(const C::TypeRef &type);

    [[nodiscard]] C::SpriteRef *getSprite() const;
    void setSprite(const C::SpriteRef &sprite);

    [[nodiscard]] C::TimerRef *getTimer() const;
    void setTimer(const C::TimerRef &timer);

protected:
    C::PositionRef *_position;
    C::SizeRef *_size;
    C::TypeRef *_type;
    C::SpriteRef *_sprite;
    C::TimerRef *_timer;
};

// ENTITY POOL
class DecorSquarePool : public AEntityPool {
public:
    DecorSquarePool();
    ~DecorSquarePool() override = default;

    // default rule of five
    DecorSquarePool(const DecorSquarePool &other) = default;
    DecorSquarePool &operator=(const DecorSquarePool &other) = default;
    DecorSquarePool(DecorSquarePool &&other) = default;
    DecorSquarePool &operator=(DecorSquarePool &&other) = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::DecorSquareRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;

protected:
    C::PositionPool _positionPool;
    C::SizePool _sizePool;
    C::TypePool _typePool;
    C::SpritePool _spritePool;
    C::TimerPool _timerPool;
};
}
