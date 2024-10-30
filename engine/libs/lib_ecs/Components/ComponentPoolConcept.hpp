/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ComponentPool
*/

#pragma once

#include "lib_ecs/Chunks/StandardChunkPool.hpp"
#include "lib_ecs/Components/ComponentRef.hpp"
#include <tuple>
#include <concepts>

namespace ECS::C {

template<typename T>
concept ComponentPoolConcept = requires(
    T t, typename T::Types types, typename T::VTypes vtypes, typename T::ATypes atypes,
    const char *name
) {
    // Required type aliases
    typename T::Types;
    typename T::VTypes;
    typename T::ATypes;

    // Required member functions
    {
        t.getComponentRef(std::declval<Chunks::chunkPos_t>())
    } -> std::same_as<ComponentRef<typename T::Types>>;
    {
        t.getComponentVal(std::declval<Chunks::chunkPos_t>())
    } -> std::same_as<ComponentVal<typename T::Types>>;
    { t.elemCount() } -> std::same_as<Chunks::chunk_idx_t>;
    { t.chunkCount() } -> std::same_as<Chunks::chunk_idx_t>;
    { t.addChunk(std::declval<size_t>()) } -> std::same_as<void>;
    { t.getChunkPools() } -> std::same_as<std::tuple<Chunks::StandardChunkPool<typename T::Types>>>;
    { t.getRawStdVectors(std::declval<size_t>()) } -> std::same_as<typename T::VTypes>;
    { t.getRawAdrrStdVectors(std::declval<size_t>()) } -> std::same_as<typename T::ATypes>;

    // Constant variable
    { T::componentName } -> std::convertible_to<const char *>;
};

} // namespace ECS::C