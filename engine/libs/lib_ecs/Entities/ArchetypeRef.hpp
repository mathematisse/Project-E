/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/ComponentRef.hpp"
#include "lib_ecs/Entities/BaseArchetype.hpp"
#include "lib_ecs/Entities/AArchetypeRef.hpp"
#include <tuple>

namespace ECS::E {

template<typename... TComps>
// requires(C::ArchComponentConcept<TComps> && ...)
class ArchetypeRef : public BaseArchetypeRef<TComps...>, public AArchetypeRef {
public:
    ArchetypeRef() = delete;
    explicit ArchetypeRef(typename TComps::Comp::Ref... input):
        BaseArchetypeRef<TComps...>(input...)
    {
    }
    explicit ArchetypeRef(std::tuple<typename TComps::Comp::Ref...> input):
        BaseArchetypeRef<TComps...>(input)
    {
    }
    ~ArchetypeRef() override = default;

    [[nodiscard]] C::entity_status_t getEntityStatusElem() const override
    {
        return this->getEntityStatusVal();
    }
    [[nodiscard]] Chunks::chunkPos_t getChunkPosElem() const override
    {
        return this->getChunkPosVal();
    }
    void setEntityStatusElem(C::entity_status_t status) override { this->setEntityStatus(status); }
    void setChunkPosElem(Chunks::chunkPos_t cPos) override
    {
        this->setChunkPos({std::get<0>(cPos), std::get<1>(cPos)});
    }
};

template<typename Tuple>
struct TupArchetypeRef;

template<typename... Types>
struct TupArchetypeRef<std::tuple<Types...>> {
    using type = ArchetypeRef<Types...>;
};

} // namespace ECS::E