/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for Decor entity declarations
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"

namespace ECS::C {
DECLARE_COMPONENT(Position, float, float);
DECLARE_COMPONENT(Size, int, int);
DECLARE_COMPONENT(Sprite, size_t);
}

namespace ECS::E {
const size_t DecorChunkSize = 1024;

// ENTITY REF
class DecorRef : public AEntityRef {
public:
    DecorRef(
        C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *position,
        C::SizeRef *size, C::SpriteRef *sprite
    );
    ~DecorRef() override;

    // Default rule of five
    DecorRef(const DecorRef &other) = default;
    DecorRef &operator=(const DecorRef &other) = default;
    DecorRef(DecorRef &&other) = default;
    DecorRef &operator=(DecorRef &&other) = default;

    // Accessors

    [[nodiscard]] C::PositionRef *getPosition() const;
    void setPosition(const C::PositionRef &position);

    [[nodiscard]] C::SizeRef *getSize() const;
    void setSize(const C::SizeRef &size);

    [[nodiscard]] C::SpriteRef *getSprite() const;
    void setSprite(const C::SpriteRef &sprite);

protected:
    C::PositionRef *_position;
    C::SizeRef *_size;
    C::SpriteRef *_sprite;
};

// ENTITY POOL
class DecorPool : public AEntityPool {
public:
    DecorPool();
    ~DecorPool() override = default;

    // default rule of five
    DecorPool(const DecorPool &other) = default;
    DecorPool &operator=(const DecorPool &other) = default;
    DecorPool(DecorPool &&other) = default;
    DecorPool &operator=(DecorPool &&other) = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::DecorRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;

protected:
    C::PositionPool _positionPool;
    C::SizePool _sizePool;
    C::SpritePool _spritePool;
};
}
