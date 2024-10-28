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

using StaticEntityAspect = Aspect<std::tuple<C::Position, C::Rotation>>;
using DynamicEntityAspect =
    Aspect<std::tuple<C::Velocity, C::Acceleration>, std::tuple<StaticEntityAspect>>;

} // namespace ECS::E
