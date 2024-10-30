/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ComponentPool
*/

#pragma once

#include "lib_ecs/Components/ComponentVal.hpp"
#include <tuple>
#include <concepts>

namespace ECS::C {

template<typename T, typename... Ts>
concept ComponentRefConcept =
    requires(T t, const T ct, std::tuple<Ts *...> components, ComponentVal<Ts...> val) {
        // Constructor with pointers to component types
        T(components);

        // Access function template `get` for each index
        { t.template get<0>() } -> std::same_as<decltype(std::get<0>(components))>;
        { ct.template get<0>() } -> std::same_as<decltype(std::get<0>(components))>;

        // Conversion operator and set functions
        { static_cast<ComponentVal<Ts...>>(t) } -> std::same_as<ComponentVal<Ts...>>;
        { t.set(val) } -> std::same_as<void>;
    };

} // namespace ECS::C