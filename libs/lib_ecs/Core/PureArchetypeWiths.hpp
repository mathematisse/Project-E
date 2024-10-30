/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "lib_ecs/Core/ArchetypeWithValsMaker.hpp"
#include "lib_ecs/Core/ArchetypeWithRefsMaker.hpp"
#include "lib_ecs/Core/ArchetypeWithPoolsMaker.hpp"

namespace ECS::E {
DECLARE_ARCHETYPE_VAL_WITH_COMPONENT(EntityStatus);
DECLARE_ARCHETYPE_VAL_WITH_COMPONENT(ChunkPos);
DECLARE_ARCHETYPE_VAL_WITH_COMPONENT(EntityPoolId);
DECLARE_ARCHETYPE_REF_WITH_COMPONENT(EntityStatus);
DECLARE_ARCHETYPE_REF_WITH_COMPONENT(ChunkPos);
DECLARE_ARCHETYPE_REF_WITH_COMPONENT(EntityPoolId);
DECLARE_ARCHETYPE_POOL_WITH_COMPONENT(EntityStatus);
DECLARE_ARCHETYPE_POOL_WITH_COMPONENT(ChunkPos);
DECLARE_ARCHETYPE_POOL_WITH_COMPONENT(EntityPoolId);
}
