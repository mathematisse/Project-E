/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "lib_ecs/Core/RawComponentMaker.hpp"
#include "lib_ecs/Core/Types.hpp"

namespace ECS::C {
DECLARE_RAW_COMPONENT(EntityStatus, entity_status_t);
DECLARE_RAW_COMPONENT(ChunkPos, Chunks::chunk_idx_t, Chunks::elem_idx_t);
DECLARE_RAW_COMPONENT(EntityPoolId, entity_pool_id_t);
}
