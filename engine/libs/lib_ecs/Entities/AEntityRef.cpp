/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntity
*/

#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"

namespace ECS {
namespace E {
AEntityRef::AEntityRef(C::EntityStatusRef status, C::ChunkPosRef cPos):
    EntityWithEntityStatusRef(status),
    EntityWithChunkPosRef(cPos)
{
}

C::entity_status_t AEntityRef::getStatusElem() const { return *_EntityStatus.get<0>(); }

Chunks::chunkPos_t AEntityRef::getChunkPosElem() const
{
    return {*_ChunkPos.get<0>(), *_ChunkPos.get<1>()};
}

void AEntityRef::setStatusElem(C::entity_status_t status) { *_EntityStatus.get<0>() = status; }

void AEntityRef::setChunkPosElem(Chunks::chunkPos_t cPos)
{
    _ChunkPos.set<1>(std::get<1>(cPos));
    _ChunkPos.set<0>(std::get<0>(cPos));
}
} // namespace E
} // namespace ECS
