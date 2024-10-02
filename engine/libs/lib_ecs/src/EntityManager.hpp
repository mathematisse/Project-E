/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base EntityManager
*/

#pragma once

#include "Chunks/ChunkPos.hpp"
#include "Entities/PureEntityPools.hpp"
#include "Entities/PureEntities.hpp"
#include "Entities/IEntityRef.hpp"
#include "Entities/IEntityPool.hpp"
#include "Systems/SystemTree.hpp"
#include <memory>
#include <vector>

namespace ECS
{
    class EntityManager
    {
    public:
        EntityManager();
        ~EntityManager();
        EntityManager(const EntityManager &other) = default;
        EntityManager(EntityManager &&other) = default;
        EntityManager &operator=(const EntityManager &other) = default;
        EntityManager &operator=(EntityManager &&other) = default;

        bool registerSystemGroup(int group, int neighbourGroup, bool addBefore = false, bool addInside = false);
        bool registerSystem(Systems::ISystem &system, int group, bool atStart = false);
        bool registerEntityPool(Entities::IEntityPool *entityPool);
        std::unique_ptr<Entities::IEntityRef> getEntity(const Entities::EntityPtrRef &entityPtr);
        std::unique_ptr<Entities::IEntityRef> getEntity(const Chunks::ChunkPos &cPos);
        Chunks::ChunkPos createEntity(const std::string &entityName);
        std::vector<Chunks::ChunkPos> createEntities(const std::string &entityName, size_t count = 0);
        void destroyEntity(const Chunks::ChunkPos &cPos);
        void destroyEntities(const std::vector<Chunks::ChunkPos> &cPosArr);

        void runSystems();
        
    private:
        std::vector<Chunks::ChunkPos> _createEntities(Entities::IEntityPool *entityPool, size_t count, size_t poolId);

        Entities::EntityPtrPool _entityPtrPool;
        std::vector<Entities::IEntityPool *> _entityPools;
        Systems::SystemTree _systemTree;
    };
}
