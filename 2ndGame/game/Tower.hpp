/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for Tower entity declarations
*/

#pragma once

#include <raylib.h>
#include "Decor.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"

static const std::vector<std::string> tower_names {"None", "Bomb", "Archer", "Wizard"};

static const std::vector<std::vector<size_t>> tower_range {
    {0, 0, 0}, {300, 300, 300}, {300, 350, 400}, {250, 300, 350}
};

enum towerType {
    NONE,
    BOMB,
    ARCHER,
    WIZARD,
};

struct tower_info {
    size_t id;
    size_t level;
    towerType type;
    Vector2 pos;
    float delay;
};

namespace ECS::C {

DECLARE_COMPONENT(Type, towerType);
DECLARE_COMPONENT(Level, size_t);
DECLARE_COMPONENT(ID, size_t);
}

namespace ECS::E {
const size_t TowerChunkSize = 32;

// ENTITY REF
class TowerRef : public AEntityRef {
public:
    TowerRef(
        C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::PositionRef *position,
        C::SizeRef *size, C::SpriteRef *sprite, C::TypeRef *type, C::LevelRef *level, C::IDRef *id
    );
    ~TowerRef() override;

    // Default rule of five
    TowerRef(const TowerRef &other) = default;
    TowerRef &operator=(const TowerRef &other) = default;
    TowerRef(TowerRef &&other) = default;
    TowerRef &operator=(TowerRef &&other) = default;

    // Accessors

    [[nodiscard]] C::PositionRef *getPosition() const;
    void setPosition(const C::PositionRef &position);

    [[nodiscard]] C::SizeRef *getSize() const;
    void setSize(const C::SizeRef &size);

    [[nodiscard]] C::SpriteRef *getSprite() const;
    void setSprite(const C::SpriteRef &sprite);

    [[nodiscard]] C::TypeRef *getType() const;
    void setType(const C::TypeRef &type);

    [[nodiscard]] C::LevelRef *getLevel() const;
    void setLevel(const C::LevelRef &level);

    [[nodiscard]] C::IDRef *getID() const;
    void setID(const C::IDRef &id);

protected:
    C::PositionRef *_position;
    C::SizeRef *_size;
    C::SpriteRef *_sprite;
    C::TypeRef *_type;
    C::LevelRef *_level;
    C::IDRef *_id;
};

// ENTITY POOL
class TowerPool : public AEntityPool {
public:
    TowerPool();
    ~TowerPool() override = default;

    // default rule of five
    TowerPool(const TowerPool &other) = default;
    TowerPool &operator=(const TowerPool &other) = default;
    TowerPool(TowerPool &&other) = default;
    TowerPool &operator=(TowerPool &&other) = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::TowerRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;

protected:
    C::PositionPool _positionPool;
    C::SizePool _sizePool;
    C::SpritePool _spritePool;
    C::TypePool _typePool;
    C::LevelPool _levelPool;
    C::IDPool _idPool;
};
}
