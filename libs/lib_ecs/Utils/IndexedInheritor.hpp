/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include <cstddef>

template<size_t N, size_t I, template<size_t, size_t> class... Bases>
struct IndexedInheritorImpl;

template<
    size_t N, size_t I, template<size_t, size_t> class Base, template<size_t, size_t> class... Rest>
struct IndexedInheritorImpl<N, I, Base, Rest...> : Base<N, I>,
                                                   IndexedInheritorImpl<N, I + 1, Rest...> { };

template<size_t N, size_t I>
struct IndexedInheritorImpl<N, I> { }; // Base case

template<size_t I, template<size_t, size_t> class... Bases>
using IndexedInheritor = IndexedInheritorImpl<sizeof...(Bases), I, Bases...>;

#define INDEXED_INHERITANCE(Ts) IndexedInheritor<0, Ts...>
