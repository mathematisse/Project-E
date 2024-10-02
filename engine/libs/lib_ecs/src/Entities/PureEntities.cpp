/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntities
*/

#include "Entities/PureEntities.hpp"


    namespace ECS::Entities
    {
        EntityPtrRef::EntityPtrRef(Components::EntityStatusRef *status, Components::ChunkPosRef *cPos, Components::EntityPoolIdRef *poolId)
            : AEntityRef(status, cPos), _poolId(poolId)
        {
        }

        EntityPtrRef::~EntityPtrRef() {
            delete _poolId;
        }

        Components::entity_pool_id_t EntityPtrRef::getPoolId() const
        {
            return *_poolId->get<0>();
        }

        void EntityPtrRef::setPoolId(Components::entity_pool_id_t poolId)
        {
            *_poolId->get<0>() = poolId;
        }
    }

