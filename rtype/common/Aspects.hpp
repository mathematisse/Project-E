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

using DecorEntityAspect = Aspect<std::tuple<C::ArchType>, std::tuple<SpriteEntityAspect>>;
using AnimatedDecorEntityAspect = Aspect<std::tuple<C::ArchType>, std::tuple<AnimatedEntityAspect>>;

using BaseEntityAspect = Aspect<
    std::tuple<
        C::ArchColor, C::ArchType, C::ArchNetworkID, C::ArchCanShoot, C::ArchIsShooting,
        C::ArchHealth, C::ArchWeapon, C::ArchSize>,
    std::tuple<DynamicEntityAspect>>;

using GameEntityAspect = Aspect<std::tuple<>, std::tuple<BaseEntityAspect, SpriteEntityAspect>>;
using AnimatedGameEntityAspect =
    Aspect<std::tuple<>, std::tuple<BaseEntityAspect, AnimatedEntityAspect>>;

} // namespace ECS::E
