/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/ComponentConcept.hpp"
#include "lib_ecs/Components/ComponentRef.hpp"
#include "lib_ecs/Components/ComponentVal.hpp"
#include "lib_ecs/Components/IComponentPool.hpp"
#include "lib_ecs/Entities/IArchetypes.hpp"
#include "lib_ecs/Utils/TupleClass.hpp"
#include "lib_ecs/Utils/IndexedInheritor.hpp"
#include "lib_ecs/Core/Types.hpp"
#include <tuple>

namespace ECS::E {

// ArchetypePool class template
template<typename Tuple>
class Archetype;

template<typename... TComps>
    requires(C::ComponentConcept<TComps> && ...)
class Archetype<std::tuple<TComps...>> {
public:
    using ComponentTypes = std::tuple<TComps...>;

    using ValTypes = std::tuple<typename TComps::Val...>;
    using RefTypes = std::tuple<typename TComps::Ref...>;
    using PoolTypes = std::tuple<typename TComps::Pool...>;
    // using ArchValTypes = std::tuple<typename TComps::ArchVal...>;
    // using ArchRefTypes = std::tuple<typename TComps::ArchRef...>;
    // using ArchPoolTypes = std::tuple<typename TComps::ArchPool...>;
};

template<typename TTupClass, typename TAccessor>
class BaseArchetype : public TupleClass<TTupClass>, public TAccessor {
public:
    BaseArchetype():
        TupleClass<TTupClass>()
    {
    }

    explicit BaseArchetype(TTupClass elems):
        TupleClass<TTupClass>(elems)
    {
    }
};

#define BASE_ARCHETYPE(components, suffix)                            \
    BaseArchetype<                                                    \
        typename Archetype<std::tuple<components...>>::suffix##Types, \
        INDEXED_INHERITANCE(components::template Arch##suffix)>

template<size_t N>
class AArchetypeVal : public IArchetypeVal {
public:
    explicit AArchetypeVal(const std ::array<ECS ::C ::IComponentVal *, N> &interfaces):
        _interfaces(interfaces)
    {
    }
    ~AArchetypeVal() override = default;

    std ::array<ECS ::C ::IComponentVal *, N> &getComponentVals() { return _interfaces; }
    template<size_t I>
    constexpr ECS ::C ::IComponentVal *getComponentVal()
    {
        return _interfaces[I];
    }

    [[nodiscard]] C::entity_status_t getEntityStatusElem() const override = 0;
    [[nodiscard]] Chunks::chunkPos_t getChunkPosElem() const override = 0;
    void setEntityStatusElem(C::entity_status_t status) override = 0;
    void setChunkPosElem(Chunks::chunkPos_t cPos) override = 0;

protected:
    std ::array<ECS ::C ::IComponentVal *, N> _interfaces;
};

template<size_t N>
class AArchetypeRef : public IArchetypeRef {
public:
    explicit AArchetypeRef(const std ::array<ECS ::C ::IComponentRef *, N> &interfaces):
        _interfaces(interfaces)
    {
    }
    ~AArchetypeRef() override = default;

    std ::array<ECS ::C ::IComponentRef *, N> &getComponentRefs() { return _interfaces; }
    template<size_t I>
    constexpr ECS ::C ::IComponentRef *getComponentRef()
    {
        return _interfaces[I];
    }
    template<size_t I>
    [[nodiscard]] constexpr const ECS ::C ::IComponentRef *getComponentRef() const
    {
        return _interfaces[I];
    }
    [[nodiscard]] C::entity_status_t getEntityStatusElem() const override = 0;
    [[nodiscard]] Chunks::chunkPos_t getChunkPosElem() const override = 0;
    void setEntityStatusElem(C::entity_status_t status) override = 0;
    void setChunkPosElem(Chunks::chunkPos_t cPos) override = 0;

protected:
    std ::array<ECS ::C ::IComponentRef *, N> _interfaces;
};

template<size_t N>
class AArchetypePool : public IArchetypePool {
public:
    explicit AArchetypePool(const std ::array<ECS ::C ::IComponentPool *, N> &interfaces):
        _interfaces(interfaces)
    {
    }
    ~AArchetypePool() override = default;
    std ::array<ECS ::C ::IComponentPool *, N> &getComponentPools() { return _interfaces; }
    std::vector<C::IComponentPool *> getVecComponentPools() override = 0;
    template<size_t I>
    constexpr ECS ::C ::IComponentPool *getComponentPool()
    {
        return _interfaces[I];
    }
    [[nodiscard]] constexpr const char *getEntityName() const override = 0;
    //  std::unique_ptr<IArchetypeRef> getEntity(Chunks::chunkPos_t cPos) override = 0;
    Chunks::cPosArr_t &getFreePos() override = 0;
    Chunks::chunk_idx_t getTotalSize() override = 0;
    Chunks::chunk_idx_t getUsedSize() override = 0;
    [[nodiscard]] Chunks::chunk_idx_t getChunkCount() const override = 0;
    C::IComponentPool *getComponentPool(const std::string &name) override = 0;
    void addChunk() override = 0;

    void resetEntityAtIndex(const Chunks::chunkPos_t &index) override = 0;
    void resetEntityAtIndexes(const Chunks::cPosArr_t &indexes) override = 0;

    C::EntityStatus::Pool &getEntityStatusPoolCore() override = 0;
    C::ChunkPos::Pool &getChunkPosPoolCore() override = 0;

protected:
    std ::array<ECS ::C ::IComponentPool *, N> _interfaces;
};

} // namespace ECS::E