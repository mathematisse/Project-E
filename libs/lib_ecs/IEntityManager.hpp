/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#pragma once

#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Entities/IArchetypePool.hpp"
#include "lib_ecs/Systems/IQuery.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"

#define ENTITY_PTR_POOL_SIZE 64

namespace ECS {
class IEntityManager {
public:
    virtual ~IEntityManager() = default;

    virtual bool registerSystemNode(
        S::SystemTreeNode *node, const std::string &targetGroup, bool addBefore = false,
        bool addInside = true
    ) = 0;
    virtual bool
    registerSystem(S::ISystem &system, const std::string &group, bool atStart = false) = 0;

    virtual bool registerFixedSystemNode(
        S::SystemTreeNode *node, const std::string &targetGroup, bool addBefore = false,
        bool addInside = true
    ) = 0;
    virtual bool
    registerFixedSystem(S::ISystem &system, const std::string &group, bool atStart = false) = 0;
    virtual bool registerEntityPool(E::IArchetypePool *entityPool) = 0;
    virtual S::IQuery &initializeQuery(S::IQuery &query) = 0;
    virtual E::IArchetypePool *getEntityPool(const std::string &entityName) = 0;

    virtual void
    destroyEntities(std::span<Chunks::chunkPos_t> cPosArr, const std::string &entityName) = 0;
    virtual void destroyEntity(const Chunks::chunkPos_t &cPos) = 0;

    virtual size_t addTime(double time) = 0;

    virtual S::SystemTree &getSystemTree() = 0;
    virtual S::SystemTree &getFixedSystemTree() = 0;

protected:
    std::tuple<E::IArchetypePool *, C::entity_pool_id_t> virtual _getEntityPool(
        const std::string &entityName
    ) = 0;

    void _runSystems() { getSystemTree().runTree(); }
    void _runFixedSystems() { getFixedSystemTree().runTree(); }
    // helper template with index sequence to create entity
    template<typename TArch, size_t... Indices>
    std::array<typename TArch::Ref, sizeof...(Indices)> _createEntityRefs(
        Chunks::cPosArr_t arr, typename TArch::Pool *pool,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        return {pool->getRawEntity(arr[Indices])...};
    }

    // same but with span as input for arr
    template<typename TArch, size_t... Indices>
    std::array<typename TArch::Ref, sizeof...(Indices)> _createEntityRefs(
        std::span<Chunks::chunkPos_t> arr, typename TArch::Pool *pool,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        return {pool->getRawEntity(arr[Indices])...};
    }
};
} // namespace ECS
