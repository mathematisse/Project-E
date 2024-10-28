/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Core/Types.hpp"

namespace ECS::E {

class IArchetypeVal {
public:
    virtual ~IArchetypeVal() = default;

    [[nodiscard]] virtual C::entity_status_t getEntityStatusElem() const = 0;
    [[nodiscard]] virtual Chunks::chunkPos_t getChunkPosElem() const = 0;
    virtual void setEntityStatusElem(C::entity_status_t status) = 0;
    virtual void setChunkPosElem(Chunks::chunkPos_t cPos) = 0;
};

} // namespace ECS::E