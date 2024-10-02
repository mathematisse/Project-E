/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#include "Systems/ASystem.hpp"

namespace ECS::Systems
{

    ASystem::ASystem(const std::vector<std::string> &matches, bool isParallel)
        : _matches(matches), _isParallel(isParallel)
    {
    }

    ASystem::~ASystem() = default;

    const std::vector<std::string> &ASystem::getMatches() const
    {
        return _matches;
    }


    bool ASystem::getIsParallel() const
    {
        return _isParallel;
    }

    bool ASystem::tryAddEntityPool(Entities::IEntityPool *entityPool)
    {
        Components::IComponentPool *componentPool = nullptr;
        std::vector<Components::IComponentPool *> newComponentPools = {};

        for (const auto &match : _matches) {
            componentPool = entityPool->getComponentPool(match);
            if (componentPool == nullptr) {
               return false;
            }
            newComponentPools.push_back(componentPool);
        }
        _componentPools.push_back(newComponentPools);
        return true;
    }

}