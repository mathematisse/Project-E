/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ComponentPool
*/

#pragma once

#include <concepts>
#include <tuple>

namespace ECS::C {

template<typename T, typename... Ts>
concept ComponentValConcept = requires(T t, const T ct, std::tuple<Ts...> values) {
    // Constructor that accepts the component values
    T {Ts {}...};
    T(values);

    // Access function template `get` for each index
    { t.template get<0>() } -> std::same_as<decltype(std::get<0>(values))>;
    { ct.template get<0>() } -> std::same_as<decltype(std::get<0>(values))>;

    // Set functions for values and `ComponentVal` instance
    { t.set(values) } -> std::same_as<void>;
    { t.set(ct) } -> std::same_as<void>;
};

} // namespace ECS::C