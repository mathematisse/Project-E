/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureEntities
*/

#pragma once

#include "Chunks/ChunkPos.hpp"
#include "Components/PureComponentPools.hpp"
#include "Entities/AEntity.hpp"


    namespace ECS::Entities
    {
        class EntityPtr : public AEntity
        {
        public:
            EntityPtr(Components::ComponentRef<Components::entity_status_t> *status, Components::ComponentRef2<Chunks::chunk_pos_t> *cPos, Components::ComponentRef<Components::entity_pool_id_t> *poolId);
            ~EntityPtr() override;
            EntityPtr(const EntityPtr &other) = default;
            EntityPtr(EntityPtr &&other) = default;
            EntityPtr &operator=(const EntityPtr &other) = default;
            EntityPtr &operator=(EntityPtr &&other) = default;
            [[nodiscard]] Components::entity_pool_id_t getPoolId() const;
            void setPoolId(Components::entity_pool_id_t poolId);
        protected:
            Components::ComponentRef<Components::entity_pool_id_t> *_poolId;
        };
    }

