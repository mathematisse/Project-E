/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Entities/AArchetypeVal.hpp"
#include "lib_ecs/Entities/AArchetypeRef.hpp"
#include "lib_ecs/Entities/AArchetypePool.hpp"
#include <concepts>

namespace ECS::E {

template<typename T, typename ComponentType>
concept ArchetypeValWithComponentConcept = std::derived_from<T, AArchetypeVal>;

template<typename T, typename ComponentType>
concept ArchetypeRefWithComponentConcept = std::derived_from<T, AArchetypeRef>;

template<typename T, typename ComponentType>
concept ArchetypePoolWithComponentConcept = std::derived_from<T, AArchetypePool>;

} // namespace ECS::E