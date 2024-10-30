/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "lib_ecs/Core/RawComponentMaker.hpp" // IWYU pragma: keep
#include "lib_ecs/Core/RawArchComponentMaker.hpp" // IWYU pragma: keep
#include "lib_ecs/Core/ArchetypeWithValsMaker.hpp" // IWYU pragma: keep
#include "lib_ecs/Core/ArchetypeWithRefsMaker.hpp" // IWYU pragma: keep
#include "lib_ecs/Core/ArchetypeWithPoolsMaker.hpp" // IWYU pragma: keep

#define DECLARE_COMPONENT(name, types...)        \
    namespace ECS {                              \
    namespace C {                                \
    DECLARE_RAW_COMPONENT(name, types);          \
    }                                            \
    namespace E {                                \
    DECLARE_ARCHETYPE_VAL_WITH_COMPONENT(name);  \
    DECLARE_ARCHETYPE_REF_WITH_COMPONENT(name);  \
    DECLARE_ARCHETYPE_POOL_WITH_COMPONENT(name); \
    }                                            \
    }                                            \
    namespace ECS {                              \
    namespace C {                                \
    DECLARE_RAW_ARCH_COMPONENT(name, types);     \
    }                                            \
    }
