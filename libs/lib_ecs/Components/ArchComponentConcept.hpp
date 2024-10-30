/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ComponentPool
*/

#pragma once

#include "lib_ecs/Components/ComponentConcept.hpp"
#include <concepts>

namespace ECS::E {
class AArchetypeVal;
class AArchetypeRef;
class AArchetypePool;
}

namespace ECS::C {

// Main ComponentConcept without template parameters
template<typename T>
concept ArchComponentConcept = requires {
    // Check that the component has `Val`, `Ref`, and `Pool` types defined
    typename T::Comp;
    // typename T::ArchVal;
    // typename T::ArchRef;
    // typename T::ArchPool;

    // requires ComponentConcept<typename T::Comp>;

    // requires std::derived_from<typename T::ArchVal, E::AArchetypeVal>;
    // requires std::derived_from<typename T::ArchRef, E::AArchetypeRef>;
    // requires std::derived_from<typename T::ArchPool, E::AArchetypePool>;
};

} // namespace ECS::C
