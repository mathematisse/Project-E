/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/ComponentVal.hpp"
#include "lib_ecs/Entities/AArchetypes.hpp"
#include <tuple>

namespace ECS::E {

template<typename... TComps>
    requires(C::ComponentConcept<TComps> && ...)
class ArchetypeVal : public BASE_ARCHETYPE(TComps, Val),
                     virtual public AArchetypeVal<sizeof...(TComps)> {
public:
    ArchetypeVal():
        AArchetypeVal<sizeof...(TComps)>(this->template getInterfacePtr<ECS ::C ::IComponentVal>()),
        BASE_ARCHETYPE(TComps, Val)()
    {
        this->_interfaces = this->template getInterfacePtr<ECS ::C ::IComponentVal>();
    }
    explicit ArchetypeVal(const TComps ::Val &...input):
        AArchetypeVal<sizeof...(TComps)>(this->template getInterfacePtr<ECS ::C ::IComponentVal>()),
        BASE_ARCHETYPE(TComps, Val)()
    {
        this->_interfaces = this->template getInterfacePtr<ECS ::C ::IComponentVal>();
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