/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "Components/AComponentPool.hpp"
#include <iostream>

#define DECLARE_COMPONENT(name, types...)                                     \
    inline constexpr char name##ComponentName[] = #name;                      \
    class name##Pool : public AComponentPool<name##ComponentName, types> { }; \
    using name##Ref = ComponentRef<types>;

namespace ECS::C {

using entity_pool_id_t = uint8_t;

using entity_status_t = int8_t;

// Attached to all entities (appart from entity ptrs) to tell the manager what to do
DECLARE_COMPONENT(EntityStatus, entity_status_t);

// Serves multiple purposes, in entity ptrs or for known pool entities references
DECLARE_COMPONENT(ChunkPos, Chunks::chunk_idx_t, Chunks::elem_idx_t);

// Identifies the entity pool in the core manager
DECLARE_COMPONENT(EntityPoolId, entity_pool_id_t);

enum EntityStatusEnum : entity_status_t {
    ENT_ERROR = -1,
    ENT_NONE = 0,
    ENT_NEEDS_INIT = 1,
    ENT_NEEDS_DESTROY = 2,
    ENT_ALIVE = 3,
};
} // namespace ECS::C

// override std::cout << "EntityStatusEnum" << std::endl;

inline std::ostream &operator<<(std::ostream &os, ECS::C::EntityStatusEnum status)
{
    switch (status) {
    case ECS::C::EntityStatusEnum::ENT_ERROR:
        os << "ENT_ERROR";
        break;
    case ECS::C::EntityStatusEnum::ENT_NONE:
        os << "ENT_NONE";
        break;
    case ECS::C::EntityStatusEnum::ENT_NEEDS_INIT:
        os << "ENT_NEEDS_INIT";
        break;
    case ECS::C::EntityStatusEnum::ENT_NEEDS_DESTROY:
        os << "ENT_NEEDS_DESTROY";
        break;
    case ECS::C::EntityStatusEnum::ENT_ALIVE:
        os << "ENT_ALIVE";
        break;
    default:
        os << "UNKNOWN";
        break;
    }
    return os;
}
