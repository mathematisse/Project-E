/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntity
*/

#pragma once

#include "Components/ComponentRefs.hpp"
#include "Components/PureComponentPools.hpp"
#include "Entities/IEntity.hpp"
#include <cstdint>


    namespace ECS::Entities
    {
        /**
         * @class AEntity
         * @brief Abstract base class for entities in the ECS framework.
         * 
         * The AEntity class provides a common interface for all entities, including methods for
         * getting and setting the status and chunk position of the entity.
         */
        class AEntity : public IEntity
        {
        public:
            /**
             * @brief Constructs an AEntity object.
             * 
             * @param status Pointer to a ComponentRef object representing the status of the entity.
             * @param cPos Pointer to a ComponentRef2 object representing the chunk position of the entity.
             */
            AEntity(Components::ComponentRef<Components::entity_status_t> *status, Components::ComponentRef2<Chunks::chunk_pos_t> *cPos);

            /**
             * @brief Destroys the AEntity object.
             */
            ~AEntity() override;

            AEntity(const AEntity &other) = default;
            AEntity(AEntity &&other) = default;
            AEntity &operator=(const AEntity &other) = default;
            AEntity &operator=(AEntity &&other) = default;

            /**
             * @brief Gets the status of the entity.
             * 
             * @return The status of the entity as a entity_status_t.
             */
            [[nodiscard]] Components::entity_status_t getStatus() const override;

            /**
             * @brief Gets the chunk position of the entity.
             *
             * @return The chunk position of the entity as a Chunks::ChunkPos.
             */
            [[nodiscard]] Chunks::ChunkPos getChunkPos() const override;

            /**
             * @brief Sets the status of the entity.
             * 
             * @param status The new status of the entity as a entity_status_t.
             */
            void setStatus(Components::entity_status_t status) override;

            /**
             * @brief Sets the chunk position of the entity.
             * 
             * @param cPos The new chunk position of the entity as a Chunks::ChunkPos.
             */
            void setChunkPos(Chunks::ChunkPos cPos) override;

            Components::ComponentRef<Components::entity_status_t> *_status; ///< Pointer to the status component of the entity.
            Components::ComponentRef2<Chunks::chunk_pos_t> *_cPos; ///< Pointer to the chunk position component of the entity.
        };
    }

