/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Entities/ArchetypeVal.hpp" // IWYU pragma: keep
#include "lib_ecs/Entities/ArchetypeRef.hpp" // IWYU pragma: keep
#include "lib_ecs/Entities/ArchetypePool.hpp" // IWYU pragma: keep

#define DECLARE_ARCHETYPE(name, aspect)                                                   \
    namespace ECS::E {                                                                    \
    inline constexpr char name##ArchetypeName[] = #name;                                  \
    class name {                                                                          \
    public:                                                                               \
        consteval static const char *getArchetypeName() { return name##ArchetypeName; }   \
        using Val = TupArchetypeVal<aspect::ArchComponents>::type;                        \
        using Ref = TupArchetypeRef<aspect::ArchComponents>::type;                        \
        using Pool = TupArchetypePool<name##ArchetypeName, aspect::ArchComponents>::type; \
    };                                                                                    \
    }
