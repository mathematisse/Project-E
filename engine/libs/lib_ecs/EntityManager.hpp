/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#pragma once

#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Entities/IArchetypes.hpp"
#include "lib_ecs/Core/PureArchetypes.hpp"
#include "lib_ecs/Core/PureComponents.hpp"
#include "lib_ecs/Systems/IQuery.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include <vector>

#define ENTITY_PTR_POOL_SIZE 64

namespace ECS {
class EntityManager {
public:
    explicit EntityManager(float fixedUpdateTime);

    bool registerSystemGroup(
        const std::string &targetGroup, const std::string &newGroup, bool addBefore = false,
        bool addInside = true
    );
    bool registerSystemNode(
        S::SystemTreeNode &node, const std::string &targetGroup, bool addBefore = false,
        bool addInside = true
    );
    bool registerSystem(S::ISystem &system, const std::string &group, bool atStart = false);
    bool registerFixedSystemGroup(
        const std::string &targetGroup, const std::string &newGroup, bool addBefore = false,
        bool addInside = true
    );
    bool registerFixedSystemNode(
        S::SystemTreeNode &node, const std::string &targetGroup, bool addBefore = false,
        bool addInside = true
    );
    bool registerFixedSystem(S::ISystem &system, const std::string &group, bool atStart = false);
    bool registerEntityPool(E::IArchetypePool *entityPool);
    S::IQuery &initializeQuery(S::IQuery &query);
    // std::unique_ptr<E::IArchetypeRef> getEntity(const E::EntityPtr::Ref &entityPtr);
    // std::unique_ptr<E::IArchetypeRef> getEntity(const Chunks::chunkPos_t &cPos);
    E::IArchetypePool *getEntityPool(const std::string &entityName);
    // Chunks::chunkPos_t
    // createEntity(const std::string &entityName, C::EntityStatusEnum status = C::ENT_ALIVE);
    // Chunks::cPosArr_t createEntities(
    //     const std::string &entityName, size_t count = 0, C::EntityStatusEnum status =
    //     C::ENT_ALIVE
    // );
    void destroyEntity(const Chunks::chunkPos_t &cPos);
    void destroyEntities(const Chunks::cPosArr_t &cPosArr);
    void destroyEntities(const Chunks::cPosArr_t &cPosArr, const std::string &entityName);

    bool addTime(float time);

    S::SystemTree &getSystemTree() { return _systemTree; };
    S::SystemTree &getFixedSystemTree() { return _fixedSystemTree; };

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
    std::array<typename TArch::Ref, N> createEntities(C::EntityStatusEnum status = C::ENT_ALIVE)
    {
        LOG_DEBUG(
            LOG_BLUE "Creating " + std::to_string(N) + " entities of type " LOG_COLOR_RESET +
            std::string(TArch::getArchetypeName())
        );
        auto ePool = _getEntityPool(TArch::getArchetypeName());
        return _createEntities<TArch, N>(ePool, status);
    }

    template<typename TArch>
    std::vector<typename TArch::Ref>
    createEntities(size_t count, C::EntityStatusEnum status = C::ENT_ALIVE)
    {
        LOG_DEBUG(
            LOG_BLUE "Creating " + std::to_string(count) + " entities of type " LOG_COLOR_RESET +
            std::string(TArch::getArchetypeName())
        );
        auto ePool = _getEntityPool(TArch::getArchetypeName());
        return _createEntities<TArch>(ePool, count, status);
    }

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
            dynamic_cast<typename TArch::Pool *>(_entityPools[entityPtr.getEntityPoolIdVal()]);
        return pool->getRawEntity(entityPtr.getChunkPosElem());
    }

private:
    void _runSystems();
    void _runFixedSystems();
    Chunks::cPosArr_t _createEntities(
        std::tuple<E::IArchetypePool *, C::entity_pool_id_t>, size_t count,
        C::EntityStatusEnum status
    );
    // Chunks::chunkPos_t _createEntity(
    //     std::tuple<E::IArchetypePool *, C::entity_pool_id_t>, C::EntityStatusEnum status
    // );
    std::tuple<E::IArchetypePool *, C::entity_pool_id_t>
    _getEntityPool(const std::string &entityName);
    void _destroyEntities(const Chunks::cPosArr_t &cPosArr, E::IArchetypePool *entityPool);

    E::EntityPtr::Pool _entityPtrPool = E::EntityPtr::Pool(ENTITY_PTR_POOL_SIZE);
    std::vector<E::IArchetypePool *> _entityPools;
    S::SystemTree _systemTree;
    S::SystemTree _fixedSystemTree; // run on fixed update
    float _timePassed = 0;
    float _timeNotAdded = 0;
    float _timeSinceLastFixedUpdate = 0;

    float _fixedUpdateTime = 0;

    template<typename TArch, size_t N>
    std::array<typename TArch::Ref, N> _createEntities(
        std::tuple<ECS::E::IArchetypePool *, C::entity_pool_id_t> ePool, C::EntityStatusEnum status
    )
    {
        auto [_entityPool, poolId] = ePool;
        auto entityPool = dynamic_cast<typename TArch::Pool *>(_entityPool);

        auto &freePos = entityPool->getFreePos();
        auto &freePtrPos = _entityPtrPool.getFreePos();

        while (freePos.size() < N) {
            entityPool->addChunk();
        }
        while (freePtrPos.size() < N) {
            _entityPtrPool.addChunk();
        }

        auto nextFreePosArr = std::vector(freePos.begin(), freePos.begin() + static_cast<long>(N));
        auto nextFreePtrPosArr =
            std::vector(freePtrPos.begin(), freePtrPos.begin() + static_cast<long>(N));

        entityPool->getEntityStatusPoolCore().setComponentAtIndexes(nextFreePosArr, status);
        entityPool->getChunkPosPoolCore().setComponentsAtIndexes(nextFreePosArr, nextFreePtrPosArr);

        _entityPtrPool.getEntityStatusPool().setComponentAtIndexes(nextFreePtrPosArr, C::ENT_ALIVE);
        _entityPtrPool.getChunkPosPool().setComponentsAtIndexes(nextFreePtrPosArr, nextFreePosArr);
        _entityPtrPool.getEntityPoolIdPool().setComponentAtIndexes(nextFreePtrPosArr, poolId);

        freePtrPos.erase(freePtrPos.begin(), freePtrPos.begin() + static_cast<long>(N));
        freePos.erase(freePos.begin(), freePos.begin() + static_cast<long>(N));

        // call helper with index sequence
        return _createEntityRefs<TArch>(nextFreePosArr, entityPool, std::make_index_sequence<N> {});
    }

    template<typename TArch>
    typename TArch::Ref _createEntity(
        std::tuple<ECS::E::IArchetypePool *, C::entity_pool_id_t> ePool, C::EntityStatusEnum status
    )
    {
        auto [_entityPool, poolId] = ePool;
        auto entityPool = dynamic_cast<typename TArch::Pool *>(_entityPool);
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
    std::vector<typename TArch::Ref> _createEntities(
        std::tuple<ECS::E::IArchetypePool *, C::entity_pool_id_t> ePool, size_t count,
        C::EntityStatusEnum status
    )
    {
        // auto [_entityPool, poolId] = ePool;
        // auto *entityPool = dynamic_cast<typename TArch::Pool *>(_entityPool);
        // auto &freePos = entityPool->getFreePos();
        // auto &freePtrPos = _entityPtrPool.getFreePos();

        // std::vector<typename TArch::Ref> res; // Results
        // res.reserve(count);

        // while (freePos.size() < count) {
        //     entityPool->addChunk();
        // }
        // while (freePtrPos.size() < count) {
        //     _entityPtrPool.addChunk();
        // }

        // auto nextFreePosArr = std::vector(freePos.begin(), freePos.begin() +
        // static_cast<long>(count)); auto nextFreePtrPosArr =
        //     std::vector(freePtrPos.begin(), freePtrPos.begin() + static_cast<long>(count));

        // entityPool->getEntityStatusPoolCore().setComponentAtIndexes(nextFreePosArr, status);
        // entityPool->getChunkPosPoolCore().setComponentsAtIndexes(nextFreePosArr,
        // nextFreePtrPosArr);

        // _entityPtrPool.getEntityStatusPool().setComponentAtIndexes(nextFreePtrPosArr,
        // C::ENT_ALIVE); _entityPtrPool.getChunkPosPool().setComponentsAtIndexes(nextFreePtrPosArr,
        // nextFreePosArr);
        // _entityPtrPool.getEntityPoolIdPool().setComponentAtIndexes(nextFreePtrPosArr, poolId);

        // freePtrPos.erase(freePtrPos.begin(), freePtrPos.begin() + static_cast<long>(count));
        // freePos.erase(freePos.begin(), freePos.begin() + static_cast<long>(count));

        // for (size_t i = 0; i < count; i++) {
        //     auto cPos = Chunks::chunkPos_t(nextFreePosArr[i]);
        //     res[i] = entityPool->getRawEntity(cPos);
        // }
        // return res;

        // above segs for unknown reasons so do a stupid for with creation one by one
        std::vector<typename TArch::Ref> res; // Results
        res.reserve(count);
        for (size_t i = 0; i < count; i++) {
            res.push_back(_createEntity<TArch>(ePool, status));
        }
        return res;
    }

    // helper template with index sequence to create entity
    template<typename TArch, size_t... Indices>
    std::array<typename TArch::Ref, sizeof...(Indices)> _createEntityRefs(
        Chunks::cPosArr_t arr, typename TArch::Pool *pool,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        return {pool->getRawEntity(arr[Indices])...};
    }

    // // helper template with index sequence to create entity
    // template <typename TArch, size_t... Indices>
    // std::vector<typename TArch::Ref> _createEntityRefs(Chunks::cPosArr_t arr, typename
    // TArch::Pool *pool, std::index_sequence<Indices...> /*unused*/)
    // {
    //     return { pool->getRawEntity(arr[Indices])... };
    // }
};
} // namespace ECS
