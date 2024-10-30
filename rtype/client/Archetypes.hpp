/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "Aspects.hpp"
#include "lib_ecs/Core/ArchetypeMaker.hpp"

DECLARE_ARCHETYPE(DecorEntity, DecorEntityAspect)

DECLARE_ARCHETYPE(AnimatedDecorEntity, AnimatedDecorEntityAspect)

DECLARE_ARCHETYPE(GameEntity, GameEntityAspect)
DECLARE_ARCHETYPE(AnimatedGameEntity, AnimatedGameEntityAspect)