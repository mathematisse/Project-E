/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include "lib_ecs/Components/ComponentPool.hpp" // IWYU pragma: keep

namespace ECS::E {
template<size_t N>
class AArchetypePool;

template<size_t N>
class AArchetypeRef;

template<size_t N>
class AArchetypeVal;
}

#define PREDECLARE(name, types...) \
    namespace ECS::C {             \
    class name;                    \
    }                              \
    namespace ECS::E {             \
    template<size_t N, size_t I>   \
    class ArchetypeValWith##name;  \
    template<size_t N, size_t I>   \
    class ArchetypeRefWith##name;  \
    template<size_t N, size_t I>   \
    class ArchetypePoolWith##name; \
    }

// ---

#define DECLARE_RAW_COMPONENT(name, ...)                              \
    inline constexpr char name##ComponentName[] = #name;              \
    class name {                                                      \
    public:                                                           \
        using Val = ComponentVal<__VA_ARGS__>;                        \
        using Ref = ComponentRef<__VA_ARGS__>;                        \
        using Pool = ComponentPool<name##ComponentName, __VA_ARGS__>; \
        template<size_t N, size_t I>                                  \
        using ArchVal = E::ArchetypeValWith##name<N, I>;              \
        template<size_t N, size_t I>                                  \
        using ArchRef = E::ArchetypeRefWith##name<N, I>;              \
        template<size_t N, size_t I>                                  \
        using ArchPool = E::ArchetypePoolWith##name<N, I>;            \
    };

// ---

#define DECLARE_ARCHETYPE_VAL_WITH_COMPONENT(name)                                    \
    template<size_t N, size_t I>                                                      \
    class ArchetypeValWith##name : virtual public AArchetypeVal<N> {                  \
    public:                                                                           \
        [[nodiscard]] C::name::Val get##name() const                                  \
        {                                                                             \
            return *static_cast<C::name::Val *>(this->template getComponentVal<I>()); \
        }                                                                             \
        void set##name(const C::name::Val &c)                                         \
        {                                                                             \
            static_cast<C::name::Val *>(this->template getComponentVal<I>())->set(c); \
        }                                                                             \
    };

// ---

#define DECLARE_ARCHETYPE_REF_WITH_COMPONENT(name)                                          \
    template<size_t N, size_t I>                                                            \
    class ArchetypeRefWith##name : virtual public AArchetypeRef<N> {                        \
    public:                                                                                 \
        [[nodiscard]] C::name::Ref get##name##Ref()                                         \
        {                                                                                   \
            return *static_cast<C::name::Ref *>(this->template getComponentRef<I>());       \
        }                                                                                   \
        [[nodiscard]] C::name::Val get##name##Val() const                                   \
        {                                                                                   \
            return *static_cast<const C::name::Ref *>(this->template getComponentRef<I>()); \
        }                                                                                   \
        void set##name(const C::name::Ref &c)                                               \
        {                                                                                   \
            static_cast<C::name::Ref *>(this->template getComponentRef<I>())->set(c);       \
        }                                                                                   \
        void set##name(C::name::Val c)                                                      \
        {                                                                                   \
            static_cast<C::name::Ref *>(this->template getComponentRef<I>())->set(c);       \
        }                                                                                   \
    };

// ---

#define DECLARE_ARCHETYPE_POOL_WITH_COMPONENT(name)                                     \
    template<size_t N, size_t I>                                                        \
    class ArchetypePoolWith##name : virtual public AArchetypePool<N> {                  \
    public:                                                                             \
        [[nodiscard]] C::name::Val get##name##Val(Chunks::chunkPos_t cPos) const        \
        {                                                                               \
            return static_cast<C::name::Pool *>(this->template getComponentPool<I>())   \
                ->getComponentVal(cPos);                                                \
        }                                                                               \
        [[nodiscard]] C::name::Ref get##name##Ref(Chunks::chunkPos_t cPos)              \
        {                                                                               \
            return static_cast<C::name::Pool *>(this->template getComponentPool<I>())   \
                ->getComponentRef(cPos);                                                \
        }                                                                               \
        [[nodiscard]] C::name::Ref getComponentRef(Chunks::chunkPos_t cPos)             \
        {                                                                               \
            return static_cast<C::name::Pool *>(this->template getComponentPool<I>())   \
                ->getComponentRef(cPos);                                                \
        }                                                                               \
        [[nodiscard]] C::name::Pool &get##name##Pool()                                  \
        {                                                                               \
            return *static_cast<C::name::Pool *>(this->template getComponentPool<I>()); \
        }                                                                               \
    };

// ---

#define DECLARE_COMPONENT(name, types...)        \
    PREDECLARE(name, types);                     \
    namespace ECS {                              \
    namespace C {                                \
    DECLARE_RAW_COMPONENT(name, types);          \
    }                                            \
    namespace E {                                \
    DECLARE_ARCHETYPE_VAL_WITH_COMPONENT(name);  \
    DECLARE_ARCHETYPE_REF_WITH_COMPONENT(name);  \
    DECLARE_ARCHETYPE_POOL_WITH_COMPONENT(name); \
    }                                            \
    }
