/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntity
*/

#pragma once

#include "Components/Components.hpp"
#include "Entities/IEntity.hpp"
#include <cstdint>


    namespace ECS::Entities
    {
        class AEntity : public IEntity
        {
        public:
            AEntity(Components::Component<uint8_t> *status, Components::Component2<uint64_t> *cPos);
            ~AEntity() override;
            AEntity(const AEntity &other) = delete;
            AEntity(AEntity &&other) = delete;
            AEntity &operator=(const AEntity &other) = delete;
            AEntity &operator=(AEntity &&other) = delete;
            [[nodiscard]] uint8_t getStatus() const override;
            [[nodiscard]] Chunks::ChunkPos getChunkPos() const override;
            void setStatus(uint8_t status) override;
            void setChunkPos(Chunks::ChunkPos cPos) override;
        protected:
            Components::Component<uint8_t> *_status;
            Components::Component2<uint64_t> *_cPos;
        };
    }

