/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#pragma once

#include "lib_ecs/AEntityManager.hpp"
#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Core/PureArchetypes.hpp"
#include "lib_ecs/Entities/IArchetypePool.hpp"
#include <vector>
#include <ranges> // Include for C++20 ranges

#define ENTITY_PTR_POOL_SIZE 64

namespace ECS {
class EntityManager : public AEntityManager {
public:
    ~EntityManager() override = default;
    explicit EntityManager(float fixedUpdateTime);

    // templated get that can raise an exception (notice no need of pool id here)
    template<typename TArch>
    typename TArch::Ref getEntity(const E::EntityPtr::Ref &entityPtr)
    {
        auto *pool =
            static_cast<typename TArch::Pool *>(_entityPools[entityPtr.getEntityPoolIdVal()]);
        return pool->getRawEntity(entityPtr.getChunkPosElem());
    }

    template<typename TArch>
    typename TArch::Ref getEntity(const Chunks::chunkPos_t &cPos)
    {
        auto entityPtr = _entityPtrPool.getRawEntity(cPos);
        auto *pool =
            static_cast<typename TArch::Pool *>(_entityPools[entityPtr.getEntityPoolIdVal()]);
        return pool->getRawEntity(entityPtr.getChunkPosElem());
    }

    void destroyEntities(std::ranges::input_range auto &&cPosRange);

    void
    _destroyEntities(std::ranges::input_range auto &&cPosRange, ECS::E::IArchetypePool *entityPool);

    template<typename TArch>
    typename TArch::Ref createEntity(C::EntityStatusEnum status = C::ENT_ALIVE)
    {
        LOG_DEBUG(
            LOG_BLUE "Creating entity of type " LOG_COLOR_RESET +
            std::string(TArch::getArchetypeName())
        );
        auto ePool = _getEntityPool(TArch::getArchetypeName());
        return _createEntity<TArch>(ePool, status);
    }

    template<typename TArch, size_t N>
    auto createEntities(C::EntityStatusEnum status = C::ENT_ALIVE)
    {
        LOG_DEBUG(
            LOG_BLUE "Creating " + std::to_string(N) + " entities of type " LOG_COLOR_RESET +
            std::string(TArch::getArchetypeName())
        );
        auto ePool = _getEntityPool(TArch::getArchetypeName());
        return _createEntities<TArch, N>(ePool, status) | std::views::take(N);
    }

    void
    destroyEntities(std::span<Chunks::chunkPos_t> cPosArr, const std::string &entityName) override;
    void destroyEntity(const Chunks::chunkPos_t &cPos) override;

    template<typename TArch>
    auto createEntities(size_t count, C::EntityStatusEnum status = C::ENT_ALIVE)
    {
        LOG_DEBUG(
            LOG_BLUE "Creating " + std::to_string(count) + " entities of type " LOG_COLOR_RESET +
            std::string(TArch::getArchetypeName())
        );
        auto ePool = _getEntityPool(TArch::getArchetypeName());
        return _createEntities<TArch>(ePool, count, status);
    }

    void deleteEverything()
    {
        for (auto &entityPool : _entityPools) {
            entityPool->deleteEverything();
        }
        _entityPtrPool.deleteEverything();
    }

protected:
    auto _createEntities(
        std::tuple<ECS::E::IArchetypePool *, C::entity_pool_id_t> ePool, size_t count,
        C::EntityStatusEnum status
    );

    E::EntityPtr::Pool _entityPtrPool;

    template<typename TArch, size_t N>
    auto _createEntities(
        std::tuple<ECS::E::IArchetypePool *, C::entity_pool_id_t> ePool, C::EntityStatusEnum status
    )
    {
        auto [_entityPool, poolId] = ePool;
        auto entityPool = static_cast<typename TArch::Pool *>(_entityPool);

        auto &freePos = entityPool->getFreePos();
        auto &freePtrPos = _entityPtrPool.getFreePos();

        while (freePos.size() < N) {
            entityPool->addChunk();
        }
        while (freePtrPos.size() < N) {
            _entityPtrPool.addChunk();
        }

        auto nextFreePosRange = freePos | std::views::take(N);
        auto nextFreePtrPosRange = freePtrPos | std::views::take(N);

        entityPool->getEntityStatusPoolCore().setComponentAtIndexes(nextFreePosRange, status);
        entityPool->getChunkPosPoolCore().setComponentsAtIndexes(
            nextFreePosRange, nextFreePtrPosRange
        );

        _entityPtrPool.getEntityStatusPool().setComponentAtIndexes(
            nextFreePtrPosRange, C::ENT_ALIVE
        );
        _entityPtrPool.getChunkPosPool().setComponentsAtIndexes(
            nextFreePtrPosRange, nextFreePosRange
        );
        _entityPtrPool.getEntityPoolIdPool().setComponentAtIndexes(nextFreePtrPosRange, poolId);

        freePtrPos.erase(freePtrPos.begin(), freePtrPos.begin() + N);
        freePos.erase(freePos.begin(), freePos.begin() + N);

        auto entities = nextFreePosRange | std::views::transform([entityPool](auto pos) {
                            return entityPool->getRawEntity(Chunks::chunkPos_t(pos));
                        });

        return entities;
    }

    template<typename TArch>
    typename TArch::Ref _createEntity(
        std::tuple<ECS::E::IArchetypePool *, C::entity_pool_id_t> ePool, C::EntityStatusEnum status
    )
    {
        auto [_entityPool, poolId] = ePool;
        auto entityPool = static_cast<typename TArch::Pool *>(_entityPool);
        auto &freePos = entityPool->getFreePos();
        auto &freePtrPos = _entityPtrPool.getFreePos();

        if (freePos.empty()) {
            entityPool->addChunk();
        }
        if (freePtrPos.empty()) {
            _entityPtrPool.addChunk();
        }
        auto nextFreePos = freePos.front();
        auto nextFreePtrPos = freePtrPos.front();

        auto entity = entityPool->getRawEntity(Chunks::chunkPos_t(nextFreePos));
        entity.setEntityStatusElem(status);
        entity.setChunkPosElem(Chunks::chunkPos_t(nextFreePtrPos));

        auto entityPtr = _entityPtrPool.getRawEntity(Chunks::chunkPos_t(nextFreePtrPos));
        entityPtr.setEntityStatusElem(C::ENT_ALIVE);
        entityPtr.setChunkPosElem(Chunks::chunkPos_t(nextFreePos));
        entityPtr.setEntityPoolId({poolId});

        freePtrPos.erase(freePtrPos.begin());
        freePos.erase(freePos.begin());

        return entity;
    }

    template<typename TArch>
    auto _createEntities(
        std::tuple<ECS::E::IArchetypePool *, C::entity_pool_id_t> ePool, size_t count,
        C::EntityStatusEnum status
    )
    {
        auto [_entityPool, poolId] = ePool;
        auto *entityPool = dynamic_cast<typename TArch::Pool *>(_entityPool);
        auto &freePos = entityPool->getFreePos();
        auto &freePtrPos = _entityPtrPool.getFreePos();

        while (freePos.size() < count) {
            entityPool->addChunk();
        }
        while (freePtrPos.size() < count) {
            _entityPtrPool.addChunk();
        }

        auto nextFreePosRange = freePos | std::views::take(count);
        auto nextFreePtrPosRange = freePtrPos | std::views::take(count);

        entityPool->getEntityStatusPoolCore().setComponentAtIndexes(nextFreePosRange, status);
        entityPool->getChunkPosPoolCore().setComponentsAtIndexes(
            nextFreePosRange, nextFreePtrPosRange
        );

        _entityPtrPool.getEntityStatusPool().setComponentAtIndexes(
            nextFreePtrPosRange, C::ENT_ALIVE
        );
        _entityPtrPool.getChunkPosPool().setComponentsAtIndexes(
            nextFreePtrPosRange, nextFreePosRange
        );
        _entityPtrPool.getEntityPoolIdPool().setComponentAtIndexes(nextFreePtrPosRange, poolId);

        freePtrPos.erase(freePtrPos.begin(), freePtrPos.begin() + static_cast<long>(count));
        freePos.erase(freePos.begin(), freePos.begin() + static_cast<long>(count));

        auto entities = nextFreePosRange | std::views::transform([entityPool](auto pos) {
                            return entityPool->getRawEntity(Chunks::chunkPos_t(pos));
                        });

        return entities;
    }
};
} // namespace ECS
