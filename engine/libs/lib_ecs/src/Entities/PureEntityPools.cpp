/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntityPools
*/

#include "Entities/PureEntityPools.hpp"
#include "Entities/IEntity.hpp"
#include "Entities/PureEntities.hpp"


    namespace ECS::Entities
    {

        EntityPtrPool::EntityPtrPool()
            : AEntityPool("entityPtr", EntityPtrPoolChunkSize)
        {
        }

        EntityPtrPool::~EntityPtrPool()
        {
        }

        IEntity* EntityPtrPool::operator[](Chunks::ChunkPos cPos)
        {
            return new EntityPtr(
                dynamic_cast<Components::Component<uint8_t> *>(_entityStatusPool[cPos].get()),
                dynamic_cast<Components::Component2<uint64_t> *>(_chunkPosPool[cPos].get()),
                dynamic_cast<Components::Component<uint8_t> *>(_entityPoolIdPool[cPos].get()));
        }

        std::vector<Components::IComponentPool *> EntityPtrPool::getComponentPools()
        {
            return {&_entityStatusPool, &_chunkPosPool, &_entityPoolIdPool};
        }
    }

