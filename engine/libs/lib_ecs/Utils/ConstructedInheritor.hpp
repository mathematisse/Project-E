/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include <cstddef>
#include <tuple>

template<typename... Args>
struct ConstructedInheritorImpl;

template<typename First, typename... Rest>
struct ConstructedInheritorImpl<First, Rest...> : First, ConstructedInheritorImpl<Rest...> {
    explicit ConstructedInheritorImpl(First first, Rest... rest):
        First(first),
        ConstructedInheritorImpl<Rest...>(rest...)
    {
    }
};

template<>
struct ConstructedInheritorImpl<> {
    ConstructedInheritorImpl() = default;
};

template<typename... Args>
using ConstructedInheritor = ConstructedInheritorImpl<Args...>;

#define CONSTRUCTED_INHERITANCE(Ts) ConstructedInheritor<Ts...>

// helper to use with a tuple instead of variadic template
template<typename Tuple>
struct TupConstructedInheritor;

template<typename... Types>
struct TupConstructedInheritor<std::tuple<Types...>> {
    using type = ConstructedInheritor<Types...>;
};

#define TUP_CONSTRUCTED_INHERITANCE(Tuple) typename TupConstructedInheritor<Tuple>::type