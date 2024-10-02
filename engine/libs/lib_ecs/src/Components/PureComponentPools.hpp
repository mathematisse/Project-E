/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "Components/AComponentPool.hpp"

#define DECLARE_COMPONENT(name, types...) \
    class name##Pool : public AComponentPool<types> {}; \
    using name##Ref = ComponentRef<types>;
    // using name##Pool = AComponentPool<types>; 



    namespace ECS::Components
    {

        using entity_pool_id_t = uint8_t;

        using entity_status_t = int8_t;


        // Attached to all entities (appart from entity ptrs) to tell the manager what to do
        DECLARE_COMPONENT(EntityStatus, entity_status_t);

        // Serves multiple purposes, in entity ptrs or for known pool entities references
        DECLARE_COMPONENT(ChunkPos, Chunks::chunk_pos_t, Chunks::chunk_pos_t);

        // Identifies the entity pool in the core manager
        DECLARE_COMPONENT(EntityPoolId, entity_pool_id_t);

        enum EntityStatusEnum : entity_status_t {
            ENT_ERROR = -1,
            ENT_NONE = 0,
            ENT_NEEDS_INIT = 1,
            ENT_NEEDS_DESTROY = 2,
            ENT_ALIVE = 3,
        };
    }

