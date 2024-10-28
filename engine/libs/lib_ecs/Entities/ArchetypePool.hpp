/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/IComponentPool.hpp"
#include "lib_ecs/Core/RawComponents.hpp"
#include "lib_ecs/Entities/BaseArchetype.hpp"
#include "lib_ecs/Entities/AArchetypePool.hpp"
#include "lib_ecs/Entities/ArchetypeRef.hpp"
#include "lib_log/log.hpp"
#include <tuple>

namespace ECS::E {

template<const char *Name, typename... TComps>
// requires(C::ArchComponentConcept<TComps> && ...)
class ArchetypePool : public BaseArchetypePool<TComps...>, public AArchetypePool {
public:
    explicit ArchetypePool(size_t chunkSize):
        _chunkSize(chunkSize)
    {
        LOG_DEBUG("Creating entity pool " + std::string(Name));
        LOG_DEBUG("Chunk size: " + std::to_string(_chunkSize));
    }
    explicit ArchetypePool(size_t chunkSize, const TComps ::Comp::Pool &...input):
        AArchetypePool(),
        BaseArchetypePool<TComps...>(input...),
        _chunkSize(chunkSize)
    {
        LOG_DEBUG("Creating entity pool " + std::string(Name));
        LOG_DEBUG("Chunk size: " + std::to_string(_chunkSize));
    }
    ~ArchetypePool() override = default;

    static constexpr const char *ArchPoolName = Name;

    [[nodiscard]] const char *getEntityName() const override { return ArchPoolName; }
    Chunks::cPosArr_t &getFreePos() override { return _freePos; }
    Chunks::chunk_idx_t getTotalSize() override { return std::get<0>(this->_elems).elemCount(); }
    Chunks::chunk_idx_t getUsedSize() override
    {
        return std::get<0>(this->_elems).elemCount() - _freePos.size();
    }
    [[nodiscard]] Chunks::chunk_idx_t getChunkCount() const override
    {
        return std::get<0>(this->_elems).chunkCount();
    }
    C::IComponentPool *getComponentPool(const std::string &name) override
    {
        for (auto *componentPool : getVecComponentPools()) {
            if (componentPool->getComponentName() == name) {
                return componentPool;
            }
        }
        return nullptr;
    }
    void addChunk() override
    {
        LOG_DEBUG("Adding chunk to entity pool " + std::string(Name));
        auto cCount = getChunkCount();
        for (auto *componentPool : getVecComponentPools()) {
            componentPool->addChunk(_chunkSize);
        }
        _freePos.reserve(_freePos.size() + _chunkSize);
        for (size_t i = 0; i < _chunkSize; i++) {
            _freePos.emplace_back(Chunks::chunkPos_t(cCount, i));
        }
    }

    // std::unique_ptr<E::IArchetypeRef> getEntity(Chunks::chunkPos_t cPos) override {
    //     return std::make_unique<E::ArchetypeRef<TComps...>>(getRawEntity(cPos));
    // }

    E::ArchetypeRef<TComps...> getRawEntity(Chunks::chunkPos_t cPos)
    {
        auto ent = getRawEntityImpl(cPos, std::index_sequence_for<TComps...> {});
        return ent;
    }

    const std::vector<C::IComponentPool *> &getVecComponentPools() override
    {
        return getVecComponentPoolsImpl(std::index_sequence_for<TComps...> {});
    }

    // helper with index sequence for pointers for getVecComponentPools
    template<std::size_t... Is>
    const std::vector<C::IComponentPool *> &
    getVecComponentPoolsImpl(std::index_sequence<Is...> /*unused*/)
    {
        if (_componentPools.empty()) { // Populate only if empty
            _componentPools = {&(std::get<Is>(this->_elems))...};
        }
        return _componentPools;
    }

    void resetEntityAtIndex(const Chunks::chunkPos_t &index) override
    {
        auto cPools = getVecComponentPools();
        for (auto *cPool : cPools) {
            cPool->resetComponentAtIndex(index);
        }
    }
    void resetEntityAtIndexes(const Chunks::cPosArr_t &indexes) override
    {
        auto cPools = getVecComponentPools();
        for (auto *cPool : cPools) {
            cPool->resetComponentAtIndexes(indexes);
        }
    }

    C::EntityStatus::Pool getEntityStatusPoolCore() override { return this->getEntityStatusPool(); }
    C::ChunkPos::Pool getChunkPosPoolCore() override { return this->getChunkPosPool(); }

    void deleteEverything() override
    {
        for (auto *componentPool : getVecComponentPools()) {
            componentPool->deleteEverything();
        }
        _freePos.clear();
    }

protected:
    std::vector<C::IComponentPool *> _componentPools;
    size_t _chunkSize;

    Chunks::cPosArr_t _freePos;

private:
    template<std::size_t... Is>
    E::ArchetypeRef<TComps...>
    getRawEntityImpl(Chunks::chunkPos_t cPos, std::index_sequence<Is...> /*unused*/)
    {
        auto components = std::tuple((std::get<Is>(this->_elems).getComponentRef(cPos))...);
        auto ent =
            E::ArchetypeRef<TComps...>((std::get<Is>(this->_elems).getComponentRef(cPos))...);
        // print type id of ent
        return ent;
    }
};

template<const char *Name, typename Tuple>
struct TupArchetypePool;

template<const char *Name, typename... Types>
struct TupArchetypePool<Name, std::tuple<Types...>> {
    using type = ArchetypePool<Name, Types...>;
};

} // namespace ECS::E