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
AEntityRef::AEntityRef(C::EntityStatusRef *status, C::ChunkPosRef *cPos):
    _status(status),
    _cPos(cPos)
{
}

AEntityRef::~AEntityRef()
{
    delete _status;
    delete _cPos;
}

C::entity_status_t AEntityRef::getStatus() const { return *_status->get<0>(); }

Chunks::chunkPos_t AEntityRef::getChunkPos() const { return {*_cPos->get<0>(), *_cPos->get<1>()}; }

void AEntityRef::setStatus(C::entity_status_t status) { *_status->get<0>() = status; }

void AEntityRef::setChunkPos(Chunks::chunkPos_t cPos)
{
    _cPos->set<1>(std::get<1>(cPos));
    _cPos->set<0>(std::get<0>(cPos));
}
} // namespace E
} // namespace ECS
