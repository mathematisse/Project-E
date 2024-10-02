/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntity
*/

#include "Entities/AEntity.hpp"
#include "Components/PureComponentPools.hpp"


    namespace ECS::Entities
    {
        AEntity::AEntity(Components::ComponentRef<Components::entity_status_t> *status, Components::ComponentRef2<Chunks::chunk_pos_t> *cPos)
            : _status(status), _cPos(cPos)
        {
        }

        AEntity::~AEntity() {
            delete _status;
            delete _cPos;
        }

        Components::entity_status_t AEntity::getStatus() const
        {
            return *_status->getX();
        }

        Chunks::ChunkPos AEntity::getChunkPos() const
        {
            return {*_cPos->getX(), *_cPos->getY()};
        }

        void AEntity::setStatus(Components::entity_status_t status)
        {
            *_status->getX() = status;
        }

        void AEntity::setChunkPos(Chunks::ChunkPos cPos)
        {
            _cPos->setX(cPos.chunkIndex);
            _cPos->setY(cPos.elemIndex);
        }
    }

