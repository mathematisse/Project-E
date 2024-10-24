/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#pragma once

#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/IEntityPool.hpp"
#include "lib_ecs/Entities/IEntityRef.hpp"
#include "lib_ecs/Entities/PureEntities.hpp"
#include "lib_ecs/Entities/PureEntityPools.hpp"
#include "lib_ecs/Systems/IQuery.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include <memory>
#include <vector>

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
    bool registerEntityPool(ECS::E::IEntityPool *entityPool);
    S::IQuery &initializeQuery(S::IQuery &query);
    std::unique_ptr<ECS::E::IEntityRef> getEntity(const ECS::E::EntityPtrRef &entityPtr);
    std::unique_ptr<ECS::E::IEntityRef> getEntity(const Chunks::chunkPos_t &cPos);
    Chunks::chunkPos_t
    createEntity(const std::string &entityName, C::EntityStatusEnum status = C::ENT_ALIVE);
    Chunks::cPosArr_t createEntities(
        const std::string &entityName, size_t count = 0, C::EntityStatusEnum status = C::ENT_ALIVE
    );
    void destroyEntity(const Chunks::chunkPos_t &cPos);
    void destroyEntities(const Chunks::cPosArr_t &cPosArr);
    void destroyEntities(const Chunks::cPosArr_t &cPosArr, const std::string &entityName);

    bool addTime(float time);

    S::SystemTree &getSystemTree() { return _systemTree; };
    S::SystemTree &getFixedSystemTree() { return _fixedSystemTree; };

private:
    void _runSystems();
    void _runFixedSystems();
    Chunks::cPosArr_t _createEntities(
        std::tuple<ECS::E::IEntityPool *, C::entity_pool_id_t>, size_t count,
        C::EntityStatusEnum status
    );
    Chunks::chunkPos_t _createEntity(
        std::tuple<ECS::E::IEntityPool *, C::entity_pool_id_t>, C::EntityStatusEnum status
    );
    std::tuple<ECS::E::IEntityPool *, C::entity_pool_id_t>
    _getEntityPool(const std::string &entityName);
    void _destroyEntities(const Chunks::cPosArr_t &cPosArr, ECS::E::IEntityPool *entityPool);

    ECS::E::EntityPtrPool _entityPtrPool;
    std::vector<ECS::E::IEntityPool *> _entityPools;
    S::SystemTree _systemTree;
    S::SystemTree _fixedSystemTree; // run on fixed update
    float _timePassed = 0;
    float _timeNotAdded = 0;
    float _timeSinceLastFixedUpdate = 0;

    float _fixedUpdateTime = 0;
};
} // namespace ECS
