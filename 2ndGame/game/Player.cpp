/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for Player entity declarations
*/

#include "Player.hpp"

// ENTITY
namespace ECS::E {

// ENTITY REF
PlayerRef::PlayerRef(C::EntityStatusRef *status, C::ChunkPosRef *cPos, C::ScoreRef *Score):
    AEntityRef(status, cPos),
    _score(Score)
{
}

PlayerRef::~PlayerRef() { delete _score; }

[[nodiscard]] C::ScoreRef *E::PlayerRef::getScore() const { return _score; }

void E::PlayerRef::setScore(const C::ScoreRef &score) { *_score = score; }

// ENTITY POOL
PlayerPool::PlayerPool():
    AEntityPool("Player", PlayerChunkSize)
{
}

std::unique_ptr<E::IEntityRef> PlayerPool::getEntity(Chunks::chunkPos_t cPos)
{
    return getRawEntity(cPos);
}

std::unique_ptr<E::PlayerRef> PlayerPool::getRawEntity(Chunks::chunkPos_t cPos)
{
    auto ptr = std::make_unique<E::PlayerRef>(
        static_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
        static_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
        static_cast<C::ScoreRef *>(_scorePool.getComponentRef(cPos))
    );
    return ptr;
}

std::vector<C::IComponentPool *> PlayerPool::getComponentPools()
{
    return {&_entityStatusPool, &_chunkPosPool, &_scorePool};
}
}
