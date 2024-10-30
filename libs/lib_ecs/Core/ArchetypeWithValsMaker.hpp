/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "lib_ecs/Entities/AArchetypeVal.hpp" // IWYU pragma: keep

// ---

#define DECLARE_ARCHETYPE_VAL_WITH_COMPONENT(name)                                           \
    class ArchetypeValWith##name {                                                           \
    public:                                                                                  \
        explicit ArchetypeValWith##name(C::name::Val *c):                                    \
            _name##Val(c)                                                                    \
        {                                                                                    \
        }                                                                                    \
        [[nodiscard]] C::name::Val get##name() const { return *_name##Val; }                 \
        void set##name(const C::name::Val &c = C::name::Val::Zero()) { _name##Val->set(c); } \
                                                                                             \
    protected:                                                                               \
        C::name::Val *_name##Val;                                                            \
    };
