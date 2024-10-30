/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "lib_ecs/Entities/AArchetypePool.hpp" // IWYU pragma: keep

#define DECLARE_ARCHETYPE_POOL_WITH_COMPONENT(name)                              \
    class ArchetypePoolWith##name {                                              \
    public:                                                                      \
        explicit ArchetypePoolWith##name(C::name::Pool *c):                      \
            _name##Pool(c)                                                       \
        {                                                                        \
        }                                                                        \
        [[nodiscard]] C::name::Val get##name##Val(Chunks::chunkPos_t cPos) const \
        {                                                                        \
            return _name##Pool->getComponentVal(cPos);                           \
        }                                                                        \
        [[nodiscard]] C::name::Ref get##name##Ref(Chunks::chunkPos_t cPos)       \
        {                                                                        \
            return _name##Pool->getComponentRef(cPos);                           \
        }                                                                        \
        [[nodiscard]] C::name::Ref getComponentRef(Chunks::chunkPos_t cPos)      \
        {                                                                        \
            return _name##Pool->getComponentRef(cPos);                           \
        }                                                                        \
        [[nodiscard]] C::name::Pool get##name##Pool() { return *_name##Pool; }   \
                                                                                 \
    protected:                                                                   \
        C::name::Pool *_name##Pool;                                              \
    };

// ---
