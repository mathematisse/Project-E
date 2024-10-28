/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Components/IComponentPool.hpp"
#include "lib_ecs/Core/PureComponents.hpp"
#include "lib_ecs/Entities/AArchetypes.hpp"
#include "lib_ecs/Entities/ArchetypeRef.hpp"
#include "lib_log/log.hpp"
#include <tuple>

namespace ECS::E {

template<const char *Name, typename... TComps>
    requires(C::ComponentConcept<TComps> && ...)
class ArchetypePool : public BASE_ARCHETYPE(TComps, Pool),
                      virtual public AArchetypePool<sizeof...(TComps)> {
public:
    explicit ArchetypePool(size_t chunkSize):
        AArchetypePool<sizeof...(TComps)>(this->template getInterfacePtr<ECS ::C ::IComponentPool>()
        ),
        BASE_ARCHETYPE(TComps, Pool)(),
        _chunkSize(chunkSize)
    {
        LOG_DEBUG("Creating entity pool " + std::string(Name));
        LOG_DEBUG("Chunk size: " + std::to_string(_chunkSize));
    }
    explicit ArchetypePool(size_t chunkSize, const TComps ::Pool &...input):
        AArchetypePool<sizeof...(TComps)>(this->template getInterfacePtr<ECS ::C ::IComponentPool>()
        ),
        BASE_ARCHETYPE(TComps, Pool)(input...),
        _chunkSize(chunkSize)
    {
        LOG_DEBUG("Creating entity pool " + std::string(Name));
        LOG_DEBUG("Chunk size: " + std::to_string(_chunkSize));
    }
    ~ArchetypePool() override = default;

    static constexpr const char *ArchPoolName = Name;

    [[nodiscard]] constexpr const char *getEntityName() const override { return Name; }
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
        for (auto *componentPool : AArchetypePool<sizeof...(TComps)>::getComponentPools()) {
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
        for (auto *componentPool : AArchetypePool<sizeof...(TComps)>::getComponentPools()) {
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
        return getRawEntityImpl(cPos, std::index_sequence_for<TComps...> {});
    }

    std::vector<C::IComponentPool *> getVecComponentPools() override
    {
        return {
            AArchetypePool<sizeof...(TComps)>::getComponentPools().begin(),
            AArchetypePool<sizeof...(TComps)>::getComponentPools().end()
        };
    }

    void resetEntityAtIndex(const Chunks::chunkPos_t &index) override
    {
        auto cPools = AArchetypePool<sizeof...(TComps)>::getComponentPools();
        for (auto *cPool : cPools) {
            cPool->resetComponentAtIndex(index);
        }
    }
    void resetEntityAtIndexes(const Chunks::cPosArr_t &indexes) override
    {
        auto cPools = AArchetypePool<sizeof...(TComps)>::getComponentPools();
        for (auto *cPool : cPools) {
            cPool->resetComponentAtIndexes(indexes);
        }
    }

    C::EntityStatus::Pool &getEntityStatusPoolCore() override
    {
        return this->getEntityStatusPool();
    }
    C::ChunkPos::Pool &getChunkPosPoolCore() override { return this->getChunkPosPool(); }

protected:
    size_t _chunkSize;

    Chunks::cPosArr_t _freePos;

private:
    template<std::size_t... Is>
    E::ArchetypeRef<TComps...>
    getRawEntityImpl(Chunks::chunkPos_t cPos, std::index_sequence<Is...> /*unused*/)
    {
        auto components = std::tuple((std::get<Is>(this->_elems).getComponentRef(cPos))...);
        return E::ArchetypeRef<TComps...>((std::get<Is>(this->_elems).getComponentRef(cPos))...);
    }
};

template<const char *Name, typename Tuple>
struct TupArchetypePool;

template<const char *Name, typename... Types>
struct TupArchetypePool<Name, std::tuple<Types...>> {
    using type = ArchetypePool<Name, Types...>;
};

} // namespace ECS::E