/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntity
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/IEntityRef.hpp"

namespace ECS::E {
/**
 * @class AEntity
 * @brief Abstract base class for entities in the ECS framework.
 *
 * The AEntity class provides a common interface for all entities, including methods for
 * getting and setting the status and chunk position of the entity.
 */
class AEntityRef : public IEntityRef,
                   public EntityWithEntityStatusRef,
                   public EntityWithChunkPosRef {
public:
    /**
     * @brief Constructs an AEntity object.
     *
     * @param status Pointer to a ComponentRef object representing the status of the entity.
     * @param cPos Pointer to a ComponentRef2 object representing the chunk position of the entity.
     */
    explicit AEntityRef(C::EntityStatusRef *status, C::ChunkPosRef *cPos);

    /**
     * @brief Destroys the AEntity object.
     */
    ~AEntityRef() override = default;

    /**
     * @brief Gets the status of the entity.
     *
     * @return The status of the entity as a entity_status_t.
     */
    [[nodiscard]] C::entity_status_t getStatusVal() const override;

    /**
     * @brief Gets the chunk position of the entity.
     *
     * @return The chunk position of the entity as a Chunks::chunkPos_t.
     */
    [[nodiscard]] Chunks::chunkPos_t getChunkPosVal() const override;

    /**
     * @brief Sets the status of the entity.
     *
     * @param status The new status of the entity as a entity_status_t.
     */
    void setStatusVal(C::entity_status_t status) override;

    /**
     * @brief Sets the chunk position of the entity.
     *
     * @param cPos The new chunk position of the entity as a Chunks::chunkPos_t.
     */
    void setChunkPosVal(Chunks::chunkPos_t cPos) override;
};
} // namespace ECS::E
