/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "Components.hpp"
#include "NetworkManager.hpp"
#include "core/Components.hpp"
#include "lib_ecs/Entities/Aspect.hpp"
#include "render/Aspects.hpp"
#include "spatial2d/Aspects.hpp"
#include "spatial2d/Components.hpp"
#include <tuple>

#define RTYPE_ENTITY_POOL_SIZE 64

namespace ECS::E {

using DecorEntityAspect = Aspect<std::tuple<C::Type>, std::tuple<SpriteEntityAspect>>;
using AnimatedDecorEntityAspect = Aspect<std::tuple<C::Type>, std::tuple<AnimatedEntityAspect>>;

using BaseEntityAspect = Aspect<
    std::tuple<
        C::Color, C::Type, C::NetworkID, C::CanShoot, C::IsShooting, C::Health, C::Weapon, C::Size>,
    std::tuple<DynamicEntityAspect>>;

using GameEntityAspect = Aspect<std::tuple<>, std::tuple<BaseEntityAspect, SpriteEntityAspect>>;
using AnimatedGameEntityAspect =
    Aspect<std::tuple<>, std::tuple<BaseEntityAspect, AnimatedEntityAspect>>;

} // namespace ECS::E
