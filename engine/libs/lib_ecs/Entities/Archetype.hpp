/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/ArchComponentConcept.hpp"
#include <tuple>

namespace ECS::E {

// ArchetypePool class template
template<typename Tuple>
class Archetype;

template<typename... TComps>
    requires(C::ArchComponentConcept<TComps> && ...)
class Archetype<std::tuple<TComps...>> {
public:
    using ComponentTypes = std::tuple<typename TComps::Comp...>;

    using ValTypes = std::tuple<typename TComps::Comp::Val...>;
    using RefTypes = std::tuple<typename TComps::Comp::Ref...>;
    using PoolTypes = std::tuple<typename TComps::Comp::Pool...>;
};

template<typename... TComps>
    requires(C::ArchComponentConcept<TComps> && ...)
struct ArchetypeBuilderHelper : public TComps::ArchPool... {
    explicit ArchetypeBuilderHelper(TComps::Comp::Pool *...pools):
        TComps::ArchPool(pools)...
    {
    }
};

} // namespace ECS::E
