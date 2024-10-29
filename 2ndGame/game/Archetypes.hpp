/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header hor Archetypes declarations
*/

#pragma once

#include "Components.hpp"
#include "lib_ecs/Entities/Aspect.hpp"
#include "render/Aspects.hpp"
#include "spatial2d/Aspects.hpp"
#include <tuple>
#include "lib_ecs/Core/ArchetypeMaker.hpp"

#define RTYPE_ENTITY_POOL_SIZE 64

namespace ECS::E {

using PlayerEntityAspect = Aspect<std::tuple<C::ArchScore>>;
using TowerEntityAspect = Aspect<
    std::tuple<C::ArchType, C::ArchLevel, C::ArchID>,
    std::tuple<StaticEntityAspect, SpriteEntityAspect>>;
using ProjectileEntityAspect = Aspect<
    std::tuple<C::ArchType, C::ArchLevel, C::ArchID, C::ArchDest>,
    std::tuple<DynamicEntityAspect, SpriteEntityAspect>>;
using EnemyEntityAspect = Aspect<
    std::tuple<C::ArchType, C::ArchLevel, C::ArchID, C::ArchDest, C::ArchHealth>,
    std::tuple<DynamicEntityAspect, SpriteEntityAspect>>;

} // namespace ECS::E

DECLARE_ARCHETYPE(DecorEntity, SpriteEntityAspect)
DECLARE_ARCHETYPE(PlayerEntity, PlayerEntityAspect)
DECLARE_ARCHETYPE(TowerEntity, TowerEntityAspect)
DECLARE_ARCHETYPE(ProjectileEntity, ProjectileEntityAspect)
DECLARE_ARCHETYPE(EnemyEntity, EnemyEntityAspect)
