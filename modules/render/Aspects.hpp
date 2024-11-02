/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "core/Components.hpp"
#include "lib_ecs/Entities/Aspect.hpp"
#include "render/Components.hpp"
#include "spatial2d/Aspects.hpp"
#include "spatial2d/Components.hpp"
#include <tuple>

namespace ECS::E {

using AnimatedEntityAspect = Aspect<
    std::tuple<C::ArchAnimatedSprite, C::ArchTimer, C::ArchSize, C::ArchRLayer>,
    std::tuple<StaticEntityAspect>>;
using SpriteEntityAspect =
    Aspect<std::tuple<C::ArchSprite, C::ArchSize, C::ArchRLayer>, std::tuple<StaticEntityAspect>>;

} // namespace ECS::E
