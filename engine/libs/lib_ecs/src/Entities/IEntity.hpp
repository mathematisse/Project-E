/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "Chunks/ChunkPos.hpp"


    namespace ECS::Entities
    {
        class IEntity
        {
        public:
            IEntity() = default;
            virtual ~IEntity() = default;
            IEntity(const IEntity &other) = delete;
            IEntity(IEntity &&other) = delete;
            IEntity &operator=(const IEntity &other) = delete;
            IEntity &operator=(IEntity &&other) = delete;
            [[nodiscard]] virtual uint8_t getStatus() const = 0;
            [[nodiscard]] virtual Chunks::ChunkPos getChunkPos() const = 0;
            virtual void setStatus(uint8_t status) = 0;
            virtual void setChunkPos(Chunks::ChunkPos cPos) = 0;
        };
    }

