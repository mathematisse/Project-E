/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/ComponentConcept.hpp"
#include "lib_ecs/Entities/IArchetypeVal.hpp"
#include "lib_ecs/Core/Types.hpp"

namespace ECS::E {

class AArchetypeVal : public IArchetypeVal {
public:
    ~AArchetypeVal() override = default;

    [[nodiscard]] C::entity_status_t getEntityStatusElem() const override = 0;
    [[nodiscard]] Chunks::chunkPos_t getChunkPosElem() const override = 0;
    void setEntityStatusElem(C::entity_status_t status) override = 0;
    void setChunkPosElem(Chunks::chunkPos_t cPos) override = 0;
};

} // namespace ECS::E
