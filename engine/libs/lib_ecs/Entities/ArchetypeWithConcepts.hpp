/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Entities/AArchetypes.hpp"
#include <concepts>

namespace ECS::E {

template<typename T, typename ComponentType, size_t N>
concept ArchetypeValWithComponentConcept = std::derived_from<T, AArchetypeVal<N>>;

template<typename T, typename ComponentType, size_t N>
concept ArchetypeRefWithComponentConcept = std::derived_from<T, AArchetypeRef<N>>;

template<typename T, typename ComponentType, size_t N>
concept ArchetypePoolWithComponentConcept = std::derived_from<T, AArchetypePool<N>>;

} // namespace ECS::E