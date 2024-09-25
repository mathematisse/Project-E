/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntity
*/

#include "Entities/AEntity.hpp"


    namespace ECS::Entities
    {
        AEntity::AEntity(Components::Component<uint8_t> *status, Components::Component2<uint64_t> *cPos)
            : _status(status), _cPos(cPos)
        {
        }

        AEntity::~AEntity()
        {
            
        }

        uint8_t AEntity::getStatus() const
        {
            return *_status->GetX();
        }

        Chunks::ChunkPos AEntity::getChunkPos() const
        {
            return {*_cPos->GetX(), *_cPos->GetY()};
        }

        void AEntity::setStatus(uint8_t status)
        {
            *_status->GetX() = status;
        }

        void AEntity::setChunkPos(Chunks::ChunkPos cPos)
        {
            std::cout << "setChunkPos" << std::endl;
            std::cout << cPos.chunkIndex << std::endl;
            std::cout << cPos.elemIndex << std::endl;
            _cPos->SetX(cPos.chunkIndex);
            _cPos->SetY(cPos.elemIndex);
        }
    }

