/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "lib_ecs/Entities/AArchetypeRef.hpp" // IWYU pragma: keep

#define DECLARE_ARCHETYPE_REF_WITH_COMPONENT(name)                                    \
    class ArchetypeRefWith##name {                                                    \
    public:                                                                           \
        explicit ArchetypeRefWith##name(C::name::Ref *c):                             \
            _name##Ref(c)                                                             \
        {                                                                             \
        }                                                                             \
        [[nodiscard]] C::name::Ref get##name##Ref() { return *_name##Ref; }           \
        [[nodiscard]] C::name::Val get##name##Val() const { return *_name##Ref; }     \
        void set##name(C::name::Ref c) { _name##Ref->set(c); }                        \
        void set##name(C::name::Val c = C::name::Val::Zero()) { _name##Ref->set(c); } \
                                                                                      \
    protected:                                                                        \
        C::name::Ref *_name##Ref;                                                     \
    };
