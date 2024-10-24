/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for Player entity declarations
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"

namespace ECS::C {
DECLARE_COMPONENT(Score, size_t);
}

namespace ECS::E {
const size_t PlayerChunkSize = 1;

// ENTITY REF
class PlayerRef : public AEntityRef {
public:
    PlayerRef(C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::ScoreRef *score);
    ~PlayerRef() override;

    // Default rule of five
    PlayerRef(const PlayerRef &other) = default;
    PlayerRef &operator=(const PlayerRef &other) = default;
    PlayerRef(PlayerRef &&other) = default;
    PlayerRef &operator=(PlayerRef &&other) = default;

    // Accessors

    [[nodiscard]] C::ScoreRef *getScore() const;
    void setScore(const C::ScoreRef &score);

protected:
    C::ScoreRef *_score;
};

// ENTITY POOL
class PlayerPool : public AEntityPool {
public:
    PlayerPool();
    ~PlayerPool() override = default;

    // default rule of five
    PlayerPool(const PlayerPool &other) = default;
    PlayerPool &operator=(const PlayerPool &other) = default;
    PlayerPool(PlayerPool &&other) = default;
    PlayerPool &operator=(PlayerPool &&other) = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::PlayerRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;

protected:
    C::ScorePool _scorePool;
};
}
