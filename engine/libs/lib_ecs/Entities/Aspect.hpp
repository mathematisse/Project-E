/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Core/PureComponents.hpp"
#include "lib_ecs/Utils/TupConcat.hpp"
#include "lib_ecs/Utils/TypeSet.hpp"
#include <tuple>

namespace ECS::E {

// Apsect class template
template<typename ComponentsTuple = std::tuple<>, typename AspectsTuple = std::tuple<>>
class Aspect;

template<typename... TComps, typename... TAspects>
class Aspect<std::tuple<TComps...>, std::tuple<TAspects...>> {
public:
    // Flatten the Components from all TAspects into a single tuple
    using FlattenedAspectComponents = TUPCAT(typename TAspects::AllComponents...);

    // Concatenate FlattenedAspectComponents and TComps into AllComponents
    using AllComponents = TUPSET(TUPCAT(FlattenedAspectComponents, std::tuple<TComps...>));

    using _baseComponents = std::tuple<C::EntityStatus, C::ChunkPos>;

    using ArchComponents = TUPSET(TUPCAT(_baseComponents, AllComponents));
};
} // namespace ECS