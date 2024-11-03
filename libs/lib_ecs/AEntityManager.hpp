/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#pragma once

#include "lib_ecs/Entities/IArchetypePool.hpp"
#include "lib_ecs/IEntityManager.hpp"
#include "lib_ecs/Systems/IQuery.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include <vector>

#define ENTITY_PTR_POOL_SIZE 64
#define ROOT_SYS_GROUP "ROOT"
#define FIXED_ROOT_SYS_GROUP "FIXED_ROOT"

namespace ECS {
class AEntityManager : public IEntityManager {
public:
    explicit AEntityManager(float fixedUpdateTime = 0);
    ~AEntityManager() override = default;
    bool registerSystemGroup(
        const std::string &targetGroup, const std::string &newGroup, bool addBefore = false,
        bool addInside = true
    ) override;
    bool registerSystemNode(
        S::SystemTreeNode &node, const std::string &targetGroup, bool addBefore = false,
        bool addInside = true
    ) override;
    bool
    registerSystem(S::ISystem &system, const std::string &group, bool atStart = false) override;
    bool registerFixedSystemGroup(
        const std::string &targetGroup, const std::string &newGroup, bool addBefore = false,
        bool addInside = true
    ) override;
    bool registerFixedSystemNode(
        S::SystemTreeNode &node, const std::string &targetGroup, bool addBefore = false,
        bool addInside = true
    ) override;
    bool registerFixedSystem(S::ISystem &system, const std::string &group, bool atStart = false)
        override;
    bool registerEntityPool(E::IArchetypePool *entityPool) override;
    S::IQuery &initializeQuery(S::IQuery &query) override;
    E::IArchetypePool *getEntityPool(const std::string &entityName) override;

    size_t addTime(double time) override;

    S::SystemTree &getSystemTree() override { return _systemTree; };
    S::SystemTree &getFixedSystemTree() override { return _fixedSystemTree; };

protected:
    std::tuple<E::IArchetypePool *, C::entity_pool_id_t>
    _getEntityPool(const std::string &entityName) override;

    std::vector<E::IArchetypePool *> _entityPools;
    S::SystemTree _systemTree;
    S::SystemTree _fixedSystemTree; // run on fixed update
    double _timePassed = 0;
    double _timeNotAdded = 0;
    double _timeSinceLastFixedUpdate = 0;

    double _fixedUpdateTime = 0;
};
} // namespace ECS
