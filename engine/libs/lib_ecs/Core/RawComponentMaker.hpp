/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "lib_ecs/Components/ComponentPool.hpp" // IWYU pragma: keep

#define DECLARE_RAW_COMPONENT(name, ...)                              \
    inline constexpr char name##ComponentName[] = #name;              \
    class name {                                                      \
    public:                                                           \
        using Val = ComponentVal<__VA_ARGS__>;                        \
        using Ref = ComponentRef<__VA_ARGS__>;                        \
        using Pool = ComponentPool<name##ComponentName, __VA_ARGS__>; \
    };

// ---
