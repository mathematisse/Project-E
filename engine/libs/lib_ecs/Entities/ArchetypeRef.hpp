/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/ComponentRef.hpp"
#include "lib_ecs/Entities/AArchetypes.hpp"
#include <tuple>

namespace ECS::E {

template<typename... TComps>
    requires(C::ComponentConcept<TComps> && ...)
class ArchetypeRef : public BASE_ARCHETYPE(TComps, Ref),
                     virtual public AArchetypeRef<sizeof...(TComps)> {
public:
    ArchetypeRef() = delete;
    // ArchetypeRef() : BASE_ARCHETYPE(TComps, Ref)()
    // {
    //     this->_interfaces = this->template getInterfacePtr<ECS ::C ::IComponentRef>();
    // }
    explicit ArchetypeRef(typename TComps::Ref... input):
        AArchetypeRef<sizeof...(TComps)>(this->template getInterfacePtr<ECS ::C ::IComponentRef>()),
        BASE_ARCHETYPE(TComps, Ref)(std::make_tuple(input...))
    {
        this->_interfaces = this->template getInterfacePtr<ECS::C::IComponentRef>();
    }
    explicit ArchetypeRef(std::tuple<typename TComps::Ref...> input):
        AArchetypeRef<sizeof...(TComps)>(this->template getInterfacePtr<ECS ::C ::IComponentRef>()),
        BASE_ARCHETYPE(TComps, Ref)(input)
    {
        this->_interfaces = this->template getInterfacePtr<ECS::C::IComponentRef>();
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