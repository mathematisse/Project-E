/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "lib_ecs/Core/ComponentMaker.hpp"
#include "lib_ecs/Core/Types.hpp"

// Attached to all entities (appart from entity ptrs) to tell the manager what to do
DECLARE_COMPONENT(EntityStatus, entity_status_t);

// Serves multiple purposes, in entity ptrs or for known pool entities references
DECLARE_COMPONENT(ChunkPos, Chunks::chunk_idx_t, Chunks::elem_idx_t);

// Identifies the entity pool in the core manager
DECLARE_COMPONENT(EntityPoolId, entity_pool_id_t);