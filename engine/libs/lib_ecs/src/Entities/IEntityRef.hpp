/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "Chunks/ChunkPos.hpp"
#include "Components/PureComponentPools.hpp"

namespace ECS::E {
/**
 * @brief Interface for an Entity in the ECS system.
 *
 * This class provides the basic interface for an entity, including methods
 * for getting and setting the status and chunk position of the entity.
 */
class IEntityRef {
public:
    /**
     * @brief Default constructor.
     */
    IEntityRef() = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~IEntityRef() = default;

    IEntityRef(const IEntityRef &other) = default;
    IEntityRef(IEntityRef &&other) = default;
    IEntityRef &operator=(const IEntityRef &other) = default;
    IEntityRef &operator=(IEntityRef &&other) = default;

    /**
     * @brief Get the status of the entity.
     *
     * @return entity_status_t The status of the entity.
     */
    [[nodiscard]] virtual C::entity_status_t getStatus() const = 0;

    /**
     * @brief Get the chunk position of the entity.
     *
     * @return Chunks::chunkPos_t The chunk position of the entity.
     */
    [[nodiscard]] virtual Chunks::chunkPos_t getChunkPos() const = 0;

    /**
     * @brief Set the status of the entity.
     *
     * @param status The new status of the entity.
     */
    virtual void setStatus(C::entity_status_t status) = 0;

    /**
     * @brief Set the chunk position of the entity.
     *
     * @param cPos The new chunk position of the entity.
     */
    virtual void setChunkPos(Chunks::chunkPos_t cPos) = 0;
};
} // namespace ECS::E
