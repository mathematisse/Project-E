/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntities
*/

#include "Entities/PureEntities.hpp"


    namespace ECS::Entities
    {
        EntityPtr::EntityPtr(Components::Component<uint8_t> *status, Components::Component2<uint64_t> *cPos, Components::Component<uint8_t> *poolId)
            : AEntity(status, cPos), _poolId(poolId)
        {
        }

        EntityPtr::~EntityPtr()
        {
        }

        uint8_t EntityPtr::getPoolId() const
        {
            return *_poolId->GetX();
        }
    }

