/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Entities/Aspect.hpp"
#include "spatial2d/Components.hpp"
#include <tuple>

namespace ECS::E {

using StaticEntityAspect = Aspect<std::tuple<C::ArchPosition, C::ArchRotation>>;

using DynamicEntityAspect =
    Aspect<std::tuple<C::ArchVelocity, C::ArchAcceleration>, std::tuple<StaticEntityAspect>>;

} // namespace ECS::E
