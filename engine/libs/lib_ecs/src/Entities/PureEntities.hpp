/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntities
*/

#pragma once

#include "Entities/AEntity.hpp"


    namespace ECS::Entities
    {
        class EntityPtr : public AEntity
        {
        public:
            EntityPtr(Components::Component<uint8_t> *status, Components::Component2<uint64_t> *cPos, Components::Component<uint8_t> *poolId);
            ~EntityPtr() override;
            EntityPtr(const EntityPtr &other) = delete;
            EntityPtr(EntityPtr &&other) = delete;
            EntityPtr &operator=(const EntityPtr &other) = delete;
            EntityPtr &operator=(EntityPtr &&other) = delete;
            [[nodiscard]] uint8_t getPoolId() const;
        protected:
            const Components::Component<uint8_t> *_poolId;
        };
    }

