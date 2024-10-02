/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntities
*/

#include "Entities/PureEntities.hpp"


    namespace ECS::Entities
    {
        EntityPtr::EntityPtr(Components::ComponentRef<Components::entity_status_t> *status, Components::ComponentRef2<Chunks::chunk_pos_t> *cPos, Components::ComponentRef<Components::entity_pool_id_t> *poolId)
            : AEntity(status, cPos), _poolId(poolId)
        {
        }

        EntityPtr::~EntityPtr() {
            delete _poolId;
        }

        Components::entity_pool_id_t EntityPtr::getPoolId() const
        {
            return *_poolId->getX();
        }

        void EntityPtr::setPoolId(Components::entity_pool_id_t poolId)
        {
            *_poolId->getX() = poolId;
        }
    }

