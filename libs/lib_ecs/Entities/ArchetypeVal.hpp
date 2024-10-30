/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Entities/BaseArchetype.hpp"
#include "lib_ecs/Entities/AArchetypeVal.hpp"
#include <tuple>

namespace ECS::E {

template<typename... TComps>
// requires(C::ArchComponentConcept<TComps> && ...)
class ArchetypeVal : public BaseArchetypeVal<TComps...>, public AArchetypeVal {
public:
    ArchetypeVal():
        BaseArchetypeVal<TComps...>()
    {
    }
    explicit ArchetypeVal(const TComps ::Val &...input):
        BaseArchetypeVal<TComps...>(input...)
    {
    }
    ~ArchetypeVal() override = default;

    [[nodiscard]] C::entity_status_t getEntityStatusElem() const override
    {
        return this->getEntityStatus();
    }
    [[nodiscard]] Chunks::chunkPos_t getChunkPosElem() const override
    {
        return this->getChunkPos();
    }
    void setEntityStatusElem(C::entity_status_t status) override { this->setEntityStatus(status); }
    void setChunkPosElem(Chunks::chunkPos_t cPos) override { this->setChunkPos(cPos); }
};

template<typename Tuple>
struct TupArchetypeVal;

template<typename... Types>
struct TupArchetypeVal<std::tuple<Types...>> {
    using type = ArchetypeVal<Types...>;
};

} // namespace ECS::E