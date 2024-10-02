/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntity
*/

#include "Entities/AEntityRef.hpp"
#include "Components/PureComponentPools.hpp"


    namespace ECS::Entities
    {
        AEntityRef::AEntityRef(Components::EntityStatusRef *status, Components::ChunkPosRef *cPos)
            : _status(status), _cPos(cPos)
        {
        }

        AEntityRef::~AEntityRef() {
            delete _status;
            delete _cPos;
        }

        Components::entity_status_t AEntityRef::getStatus() const
        {
            return *_status->get<0>();
        }

        Chunks::ChunkPos AEntityRef::getChunkPos() const
        {
            return {*_cPos->get<0>(), *_cPos->get<1>()};
        }

        void AEntityRef::setStatus(Components::entity_status_t status)
        {
            *_status->get<0>() = status;
        }

        void AEntityRef::setChunkPos(Chunks::ChunkPos cPos)
        {
            _cPos->set<0>(cPos.chunkIndex);
            _cPos->set<1>(cPos.elemIndex);
        }
    }

