/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ComponentPool
*/

#pragma once

#include "lib_ecs/Components/ComponentRef.hpp"
#include "lib_ecs/Components/ComponentVal.hpp"
#include "lib_ecs/Components/IComponentPool.hpp"
#include <concepts>

namespace ECS::C {

// Main ComponentConcept without template parameters
template<typename T>
concept ComponentConcept = requires {
    // Check that the component has `Val`, `Ref`, and `Pool` types defined
    typename T::Val;
    typename T::Ref;
    typename T::Pool;

    // Verify inheritance directly without specific type checks
    requires std::derived_from<typename T::Val, IComponentVal>;
    requires std::derived_from<typename T::Ref, IComponentRef>;
    requires std::derived_from<typename T::Pool, IComponentPool>;
};

} // namespace ECS::C
