/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "Components/AComponentPools.hpp"


    namespace ECS::Components
    {

        using entity_pool_id_t = uint8_t;
        using entity_status_t = int8_t;


        // Identifies the entity pool in the core manager
        class EntityPoolIdPool : public Components::AComponentPool<entity_pool_id_t>
        {
        public:
            EntityPoolIdPool();
            ~EntityPoolIdPool() override = default;
            EntityPoolIdPool(const EntityPoolIdPool &other) = default;
            EntityPoolIdPool(EntityPoolIdPool &&other) = default;
            EntityPoolIdPool &operator=(const EntityPoolIdPool &other) = default;
            EntityPoolIdPool &operator=(EntityPoolIdPool &&other) = default;
        };
        // Serves multiple purposes, in entity ptrs or for known pool entities references
        class ChunkPosPool : public Components::AComponentPool2<Chunks::chunk_pos_t>
        {
        public:
            ChunkPosPool();
            ~ChunkPosPool() override = default;
            ChunkPosPool(const ChunkPosPool &other) = default;
            ChunkPosPool(ChunkPosPool &&other) = default;
            ChunkPosPool &operator=(const ChunkPosPool &other) = default;
            ChunkPosPool &operator=(ChunkPosPool &&other) = default;
        };
        // Attached to all entities (appart from entity ptrs) to tell the manager what to do
        class EntityStatusPool : public AComponentPool<entity_status_t>
        {
        public:
            EntityStatusPool();
            ~EntityStatusPool() override = default;
            EntityStatusPool(const EntityStatusPool &other) = default;
            EntityStatusPool(EntityStatusPool &&other) = default;
            EntityStatusPool &operator=(const EntityStatusPool &other) = default;
            EntityStatusPool &operator=(EntityStatusPool &&other) = default;
        };
        enum EntityStatus {
            ENT_NONE = 0,
            ENT_NEEDS_INIT = 1,
            ENT_NEEDS_DESTROY = 2,
            ENT_ALIVE = 3,
        };
    }

