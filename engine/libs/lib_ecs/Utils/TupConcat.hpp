/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include <tuple>

// Metafunction to concatenate a variadic number of tuple types
template<typename... Tuples>
struct tuple_concat;

template<>
struct tuple_concat<> {
    using type = std::tuple<>;
};

template<typename Tuple>
struct tuple_concat<Tuple> {
    using type = Tuple;
};

template<typename... Ts, typename... Us, typename... Rest>
struct tuple_concat<std::tuple<Ts...>, std::tuple<Us...>, Rest...> {
    using type = typename tuple_concat<std::tuple<Ts..., Us...>, Rest...>::type;
};

#define TUPCAT(...) typename tuple_concat<__VA_ARGS__>::type