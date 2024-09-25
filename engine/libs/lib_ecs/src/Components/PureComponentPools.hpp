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
        // Identifies the entity pool in the core manager
        class EntityPoolIdPool : public Components::AComponentPool<uint8_t>
        {
        public:
            EntityPoolIdPool();
            ~EntityPoolIdPool() override = default;
            EntityPoolIdPool(const EntityPoolIdPool &other) = delete;
            EntityPoolIdPool(EntityPoolIdPool &&other) = delete;
            EntityPoolIdPool &operator=(const EntityPoolIdPool &other) = delete;
            EntityPoolIdPool &operator=(EntityPoolIdPool &&other) = delete;
        };
        // Serves multiple purposes, in entity ptrs or for known pool entities references
        class ChunkPosPool : public Components::AComponentPool2<uint64_t>
        {
        public:
            ChunkPosPool();
            ~ChunkPosPool() override = default;
            ChunkPosPool(const ChunkPosPool &other) = delete;
            ChunkPosPool(ChunkPosPool &&other) = delete;
            ChunkPosPool &operator=(const ChunkPosPool &other) = delete;
            ChunkPosPool &operator=(ChunkPosPool &&other) = delete;
        };
        // Attached to all entities (appart from entity ptrs) to tell the manager what to do
        class EntityStatusPool : public AComponentPool<int8_t>
        {
        public:
            EntityStatusPool();
            ~EntityStatusPool() override = default;
            EntityStatusPool(const EntityStatusPool &other) = delete;
            EntityStatusPool(EntityStatusPool &&other) = delete;
            EntityStatusPool &operator=(const EntityStatusPool &other) = delete;
            EntityStatusPool &operator=(EntityStatusPool &&other) = delete;
        };
        enum EntityStatus {
            ENT_NONE = 0,
            ENT_NEEDS_INIT = 1,
            ENT_NEEDS_DESTROY = 2,
            ENT_ALIVE = 3,
        };
    }

