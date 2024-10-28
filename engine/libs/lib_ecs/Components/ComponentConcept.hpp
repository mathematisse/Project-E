/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ComponentPool
*/

#pragma once

#include "lib_ecs/Components/ComponentPool.hpp"
#include "lib_ecs/Components/ComponentRef.hpp"
#include "lib_ecs/Components/ComponentVal.hpp"
#include "lib_ecs/Components/IComponentPool.hpp"
#include <concepts>

namespace ECS::C {

// Helper concept to check if `ArchVal`, `ArchRef`, and `ArchPool` are templated aliases
template<typename T>
concept HasArchValTemplate = requires { typename T::template ArchVal<1, 1>; };

template<typename T>
concept HasArchRefTemplate = requires { typename T::template ArchRef<1, 1>; };

template<typename T>
concept HasArchPoolTemplate = requires { typename T::template ArchPool<1, 1>; };

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

    // Ensure `ArchVal`, `ArchRef`, and `ArchPool` are template aliases that accept `size_t`
    // parameters
    requires HasArchValTemplate<T>;
    requires HasArchRefTemplate<T>;
    requires HasArchPoolTemplate<T>;
};

} // namespace ECS::C
