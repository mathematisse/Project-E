/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntityPools
*/

#include "Entities/PureEntityPools.hpp"
#include "Entities/IEntity.hpp"
#include "Entities/PureEntities.hpp"
#include "Components/PureComponentPools.hpp"


    namespace ECS::Entities
    {

        EntityPtrPool::EntityPtrPool()
            : AEntityPool("entityPtr", EntityPtrPoolChunkSize)
        {
        }

        EntityPtrPool::~EntityPtrPool() = default;

        std::unique_ptr<IEntity> EntityPtrPool::getEntity(Chunks::ChunkPos cPos)
        {
            return getRawEntity(cPos);
        }

        std::unique_ptr<Entities::EntityPtr> EntityPtrPool::getRawEntity(Chunks::ChunkPos cPos)
        {
            auto ptr = std::make_unique<Entities::EntityPtr>(
                dynamic_cast<Components::ComponentRef<Components::entity_status_t> *>(_entityStatusPool.getComponentRef(cPos)),
                dynamic_cast<Components::ComponentRef2<Chunks::chunk_pos_t> *>(_chunkPosPool.getComponentRef(cPos)),
                dynamic_cast<Components::ComponentRef<Components::entity_pool_id_t> *>(_entityPoolIdPool.getComponentRef(cPos)));
            
            return ptr;
        }

        std::vector<Components::IComponentPool *> EntityPtrPool::getComponentPools()
        {
            return {&_entityStatusPool, &_chunkPosPool, &_entityPoolIdPool};
        }
    }

