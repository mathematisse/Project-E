/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Utils/TupleClass.hpp"
#include "lib_ecs/Entities/Archetype.hpp" // IWYU pragma: keep

#include <tuple>
#include <type_traits>

// Assume C::ArchComponentConcept is defined appropriately
namespace C {
template<typename T>
concept ArchComponentConcept = true; // Replace with actual concept
}

namespace ECS::E {

// Helper to get the index of a type in a type list
template<typename T, typename... Ts>
struct index_of;

template<typename T, typename... Ts>
struct index_of<T, T, Ts...> : std::integral_constant<std::size_t, 0> { };

template<typename T, typename U, typename... Ts>
struct index_of<T, U, Ts...> : std::integral_constant<std::size_t, 1 + index_of<T, Ts...>::value> {
};

// Modify ArchetypeValBuilderHelper to extract pointers from TupleClass
template<typename... TComps>
    requires(C::ArchComponentConcept<TComps> && ...)
struct ArchetypeValBuilderHelper : public TComps::ArchVal... {
    explicit ArchetypeValBuilderHelper(
        TupleClass<typename Archetype<std::tuple<TComps...>>::ValTypes> &tuple
    ):
        TComps::ArchVal(get_pointer<TComps>(tuple))...
    {
    }

private:
    template<typename TComp>
    static typename TComp::Comp::Val *
    get_pointer(TupleClass<typename Archetype<std::tuple<TComps...>>::ValTypes> &tuple)
    {
        constexpr std::size_t index = index_of<TComp, TComps...>::value;
        return &tuple.template get<index>();
    }
};

// Similarly adjust ArchetypeRefBuilderHelper and ArchetypePoolBuilderHelper
template<typename... TComps>
    requires(C::ArchComponentConcept<TComps> && ...)
struct ArchetypeRefBuilderHelper : public TComps::ArchRef... {
    explicit ArchetypeRefBuilderHelper(
        TupleClass<typename Archetype<std::tuple<TComps...>>::RefTypes> &tuple
    ):
        TComps::ArchRef(get_pointer<TComps>(tuple))...
    {
    }

private:
    template<typename TComp>
    static typename TComp::Comp::Ref *
    get_pointer(TupleClass<typename Archetype<std::tuple<TComps...>>::RefTypes> &tuple)
    {
        constexpr std::size_t index = index_of<TComp, TComps...>::value;
        return &tuple.template get<index>();
    }
};

template<typename... TComps>
    requires(C::ArchComponentConcept<TComps> && ...)
struct ArchetypePoolBuilderHelper : public TComps::ArchPool... {
    explicit ArchetypePoolBuilderHelper(
        TupleClass<typename Archetype<std::tuple<TComps...>>::PoolTypes> &tuple
    ):
        TComps::ArchPool(get_pointer<TComps>(tuple))...
    {
    }

private:
    template<typename TComp>
    static typename TComp::Comp::Pool *
    get_pointer(TupleClass<typename Archetype<std::tuple<TComps...>>::PoolTypes> &tuple)
    {
        constexpr std::size_t index = index_of<TComp, TComps...>::value;
        return &tuple.template get<index>();
    }
};

template<typename... TComps>
    requires(C::ArchComponentConcept<TComps> && ...)
class BaseArchetypeVal : public TupleClass<typename Archetype<std::tuple<TComps...>>::ValTypes>,
                         public ArchetypeValBuilderHelper<TComps...> {
public:
    BaseArchetypeVal():
        TupleClass<typename Archetype<std::tuple<TComps...>>::ValTypes>(),
        ArchetypeValBuilderHelper<TComps...>(
            static_cast<TupleClass<typename Archetype<std::tuple<TComps...>>::ValTypes> &>(*this)
        )
    {
    }
};

template<typename... TComps>
    requires(C::ArchComponentConcept<TComps> && ...)
class BaseArchetypeRef : public TupleClass<typename Archetype<std::tuple<TComps...>>::RefTypes>,
                         public ArchetypeRefBuilderHelper<TComps...> {
public:
    explicit BaseArchetypeRef(typename TComps::Comp::Ref... input):
        TupleClass<typename Archetype<std::tuple<TComps...>>::RefTypes>(input...),
        ArchetypeRefBuilderHelper<TComps...>(
            static_cast<TupleClass<typename Archetype<std::tuple<TComps...>>::RefTypes> &>(*this)
        )
    {
    }
};

template<typename... TComps>
    requires(C::ArchComponentConcept<TComps> && ...)
class BaseArchetypePool : public TupleClass<typename Archetype<std::tuple<TComps...>>::PoolTypes>,
                          public ArchetypePoolBuilderHelper<TComps...> {
public:
    BaseArchetypePool():
        TupleClass<typename Archetype<std::tuple<TComps...>>::PoolTypes>(),
        ArchetypePoolBuilderHelper<TComps...>(
            static_cast<TupleClass<typename Archetype<std::tuple<TComps...>>::PoolTypes> &>(*this)
        )
    {
    }
};

} // namespace ECS::E