/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#include "lib_ecs/AEntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include "lib_log/log.hpp"
#include <string>
#include <tuple>

namespace ECS {

AEntityManager::AEntityManager(float fixedUpdateTime):
    _systemTree(ROOT_SYS_GROUP),
    _fixedSystemTree(FIXED_ROOT_SYS_GROUP),
    _fixedUpdateTime(fixedUpdateTime)
{
    if (fixedUpdateTime > 0) {
        _fixedSystemTree.setDeltaTime(fixedUpdateTime);
    }
}


bool AEntityManager::registerSystem(S::ISystem &system, const std::string &group, bool atStart)
{
    for (auto &entityPool : _entityPools) {
        system.tryAddEntityPool(entityPool);
    }
    return _systemTree.addSystem(&system, group, atStart);
}

bool AEntityManager::registerSystemNode(
    S::SystemTreeNode *node, const std::string &targetGroup, bool addBefore, bool addInside
)
{
    for (auto &entityPool : _entityPools) {
        node->registerEntityPool(entityPool);
    }
    return _systemTree.addSystemTreeNode(node, targetGroup, addBefore, addInside);
}


bool AEntityManager::registerFixedSystem(S::ISystem &system, const std::string &group, bool atStart)
{
    for (auto &entityPool : _entityPools) {
        system.tryAddEntityPool(entityPool);
    }
    return _fixedSystemTree.addSystem(&system, group, atStart);
}

bool AEntityManager::registerFixedSystemNode(
    S::SystemTreeNode *node, const std::string &targetGroup, bool addBefore, bool addInside
)
{
    for (auto &entityPool : _entityPools) {
        node->registerEntityPool(entityPool);
    }
    return _fixedSystemTree.addSystemTreeNode(node, targetGroup, addBefore, addInside);
}

bool AEntityManager::registerEntityPool(E::IArchetypePool *entityPool)
{
    _systemTree.registerEntityPool(entityPool);
    _fixedSystemTree.registerEntityPool(entityPool);
    _entityPools.push_back(entityPool);
    return true;
}

S::IQuery &AEntityManager::initializeQuery(S::IQuery &query)
{
    LOG_DEBUG(LOG_BLUE "Initializing Query" LOG_COLOR_RESET);
    for (auto &entityPool : _entityPools) {
        query.tryAddEntityPool(entityPool);
    }
    return query;
}

size_t AEntityManager::addTime(double time)
{
    if (_fixedUpdateTime == 0) {
        _timePassed += time;
        _systemTree.setDeltaTime(time);
        _fixedSystemTree.setDeltaTime(time);
        _runSystems();
        _runFixedSystems();
        return 1;
    }
    _timePassed += time;
    _timeSinceLastFixedUpdate += time;
    _systemTree.setDeltaTime(time);
    _runSystems();
    if (_timeSinceLastFixedUpdate >= _fixedUpdateTime) {
        size_t count = 0;
        while (_timeSinceLastFixedUpdate >= _fixedUpdateTime) {
            _runFixedSystems();
            _timeSinceLastFixedUpdate -= _fixedUpdateTime;
            count++;
        }
        return count;
    }
    return 0;
}

std::tuple<ECS::E::IArchetypePool *, C::entity_pool_id_t>
AEntityManager::_getEntityPool(const std::string &entityName)
{
    size_t idx = 0;
    for (auto &entityPool : _entityPools) {
        if (entityPool->getEntityName() == entityName) {
            return {entityPool, idx};
        }
        idx++;
    }
    return {nullptr, 0};
}

E::IArchetypePool *AEntityManager::getEntityPool(const std::string &entityName)
{
    for (auto &entityPool : _entityPools) {
        if (entityPool->getEntityName() == entityName) {
            return entityPool;
        }
    }
    return nullptr;
}

} // namespace ECS