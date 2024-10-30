/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#define DECLARE_RAW_ARCH_COMPONENT(name, ...)        \
    class Arch##name {                               \
    public:                                          \
        using Comp = name;                           \
        using ArchVal = E::ArchetypeValWith##name;   \
        using ArchRef = E::ArchetypeRefWith##name;   \
        using ArchPool = E::ArchetypePoolWith##name; \
    };

// ---
