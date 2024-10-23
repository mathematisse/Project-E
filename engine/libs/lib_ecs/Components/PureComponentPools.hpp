/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "lib_ecs/Components/AComponentPool.hpp"
#include <iostream>

#define DECLARE_RAW_COMPONENT(name, ...)                                            \
    inline constexpr char name##ComponentName[] = #name;                            \
    class name##Pool : public AComponentPool<name##ComponentName, __VA_ARGS__> { }; \
    using name##Ref = ComponentRef<__VA_ARGS__>;

#define DECLARE_ENTITY_POOL_WITH_COMPONENT(name)                                          \
    class EntityWith##name##Pool {                                                        \
    public:                                                                               \
        C::name##Ref *getComponentRef(Chunks::chunkPos_t cPos)                            \
        {                                                                                 \
            return reinterpret_cast<C::name##Ref *>(_##name##Pool.getComponentRef(cPos)); \
        }                                                                                 \
        C::name##Pool &getComponentPool() { return _##name##Pool; }                       \
        const C::name##Pool &getComponentPool() const { return _##name##Pool; }           \
                                                                                          \
    protected:                                                                            \
        C::name##Pool _##name##Pool;                                                      \
    };

#define DECLARE_ENTITY_REF_WITH_COMPONENT(name)                           \
    class EntityWith##name##Ref {                                         \
    public:                                                               \
        explicit EntityWith##name##Ref(C::name##Ref *c):                  \
            _##name(c)                                                    \
        {                                                                 \
        }                                                                 \
        ~EntityWith##name##Ref() { delete _##name; }                      \
        [[nodiscard]] C::name##Ref *get##name() const { return _##name; } \
        void set##name(C::name##Ref *c) { _##name = c; }                  \
                                                                          \
    protected:                                                            \
        C::name##Ref *_##name;                                            \
    };

#define DECLARE_COMPONENT(name, types...)     \
    namespace ECS {                           \
    namespace C {                             \
    DECLARE_RAW_COMPONENT(name, types);       \
    }                                         \
    namespace E {                             \
    DECLARE_ENTITY_REF_WITH_COMPONENT(name);  \
    DECLARE_ENTITY_POOL_WITH_COMPONENT(name); \
    }                                         \
    }

namespace ECS::C {

using entity_pool_id_t = uint8_t;

using entity_status_t = int8_t;

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

// Attached to all entities (appart from entity ptrs) to tell the manager what to do
DECLARE_COMPONENT(EntityStatus, entity_status_t);

// Serves multiple purposes, in entity ptrs or for known pool entities references
DECLARE_COMPONENT(ChunkPos, Chunks::chunk_idx_t, Chunks::elem_idx_t);

// Identifies the entity pool in the core manager
DECLARE_COMPONENT(EntityPoolId, entity_pool_id_t);