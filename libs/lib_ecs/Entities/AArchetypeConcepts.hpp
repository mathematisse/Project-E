/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/ComponentRef.hpp"
#include "lib_ecs/Components/ComponentVal.hpp"
#include "lib_ecs/Components/IComponentPool.hpp"
#include "lib_ecs/Components/ArchComponentConcept.hpp"
#include "lib_ecs/Core/RawComponents.hpp"
#include "lib_ecs/Core/Types.hpp"
#include "lib_ecs/Utils/TupleClass.hpp"

namespace ECS::E {

template<typename T, typename... TComps>
concept ArchetypeConcept = (C::ArchComponentConcept<TComps> && ...
                           ) && // Check each component type conforms to ComponentConcept
    requires {
        // Type aliases for component-related types
        typename T::ComponentTypes;
        typename T::ValTypes;
        typename T::RefTypes;
        typename T::PoolTypes;
        // typename T::ArchValTypes;
        // typename T::ArchRefTypes;
        // typename T::ArchPoolTypes;

        // Ensure type aliases are correctly defined as tuples of the nested types
        requires std::same_as<typename T::ComponentTypes, std::tuple<TComps...>>;
        requires std::same_as<typename T::ValTypes, std::tuple<typename TComps::Val...>>;
        requires std::same_as<typename T::RefTypes, std::tuple<typename TComps::Ref...>>;
        requires std::same_as<typename T::PoolTypes, std::tuple<typename TComps::Pool...>>;
        // requires std::same_as<typename T::ArchValTypes, std::tuple<typename TComps::ArchVal...>>;
        // requires std::same_as<typename T::ArchRefTypes, std::tuple<typename TComps::ArchRef...>>;
        // requires std::same_as<typename T::ArchPoolTypes, std::tuple<typename
        // TComps::ArchPool...>>;
    };

template<typename T, typename TTupClass, typename TAccessor>
concept BaseArchetypeConcept =
    ArchetypeConcept<TTupClass> && std::derived_from<T, TupleClass<TTupClass>> &&
    std::derived_from<T, TAccessor> && requires(T t) {
        // Ensure T has a default constructor initializing TupleClass<TTupClass>
        T();
    };

template<typename T, size_t N>
concept AArchetypeValConcept =
    requires(T t, ECS::C::entity_status_t status, Chunks::chunkPos_t pos) {
        // Ensure T provides component interfaces with `IComponentVal`
        { t.getComponentVals() } -> std::same_as<std::array<ECS::C::IComponentVal *, N> &>;
        { T::template getComponentVal<0>() } -> std::same_as<ECS::C::IComponentVal *>;

        // Methods for entity status and chunk position
        { t.getEntityStatusElem() } -> std::same_as<ECS::C::entity_status_t>;
        { t.getChunkPosElem() } -> std::same_as<Chunks::chunkPos_t>;
        { t.setEntityStatusElem(status) } -> std::same_as<void>;
        { t.setChunkPosElem(pos) } -> std::same_as<void>;
    };

template<typename T, size_t N>
concept AArchetypeRefConcept =
    requires(T t, ECS::C::entity_status_t status, Chunks::chunkPos_t pos) {
        // Ensure T provides component interfaces with `IComponentRef`
        { t.getComponentRefs() } -> std::same_as<std::array<ECS::C::IComponentRef *, N> &>;
        { T::template getComponentRef<0>() } -> std::same_as<ECS::C::IComponentRef *>;

        // Methods for entity status and chunk position
        { t.getEntityStatusElem() } -> std::same_as<ECS::C::entity_status_t>;
        { t.getChunkPosElem() } -> std::same_as<Chunks::chunkPos_t>;
        { t.setEntityStatusElem(status) } -> std::same_as<void>;
        { t.setChunkPosElem(pos) } -> std::same_as<void>;
    };

template<typename T, size_t N>
concept AArchetypePoolConcept = requires(T t, const std::string &name, Chunks::chunkPos_t pos) {
    // Ensure T provides component interfaces with `IComponentPool`
    { t.getComponentPools() } -> std::same_as<std::array<ECS::C::IComponentPool *, N> &>;
    { T::template getComponentPool<0>() } -> std::same_as<ECS::C::IComponentPool *>;

    // Pool and entity management methods
    { t.getEntityName() } -> std::same_as<const char *>;
    // { t.getEntity(pos) } -> std::same_as<std::unique_ptr<ECS::E::AArchetypeRef<N>>>;
    { t.getFreePos() } -> std::same_as<Chunks::cPosArr_t &>;
    { t.getTotalSize() } -> std::same_as<Chunks::chunk_idx_t>;
    { t.getUsedSize() } -> std::same_as<Chunks::chunk_idx_t>;
    { t.getChunkCount() } -> std::same_as<Chunks::chunk_idx_t>;
    { t.getComponentPool(name) } -> std::same_as<ECS::C::IComponentPool *>;
    { t.addChunk() } -> std::same_as<void>;

    // Entity status and chunk position pools
    { t.getEntityStatusPool() } -> std::same_as<ECS::C::EntityStatus::Pool &>;
    { t.getChunkPosPool() } -> std::same_as<ECS::C::ChunkPos::Pool &>;

    // Methods for resetting entity indices
    { t.resetEntityAtIndex(pos) } -> std::same_as<void>;
    { t.resetEntityAtIndexes(std::declval<Chunks::cPosArr_t>()) } -> std::same_as<void>;
};

} // namespace ECS::E
