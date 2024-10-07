/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IComponentPool
*/

#pragma once

#include "Chunks/ChunkPos.hpp"
#include "Components/IComponentRef.hpp"
#include <cstddef>

namespace ECS::C {
/**
 * @class IComponentPool
 * @brief Abstract base class for component pools in the ECS (Entity Component System).
 *
 * This class provides an interface for managing pools of components, including
 * methods for retrieving component references, dummy component references, and
 * information about the number of elements and chunks in the pool.
 */
class IComponentPool {
public:
    /**
     * @brief Virtual constructor for the IComponentPool class.
     */
    IComponentPool() = default;

    /**
     * @brief Virtual destructor for the IComponentPool class.
     */
    virtual ~IComponentPool() = default;

    /**
     * @brief Deleted copy constructor.
     */
    IComponentPool(const IComponentPool &other) = default;

    /**
     * @brief Deleted move constructor.
     */
    IComponentPool(IComponentPool &&other) = default;

    /**
     * @brief Deleted copy assignment operator.
     */
    IComponentPool &operator=(const IComponentPool &other) = default;

    /**
     * @brief Deleted move assignment operator.
     */
    IComponentPool &operator=(IComponentPool &&other) = default;

    /**
     * @brief Get the name of the component.
     * @return A constant reference to a string containing the component name.
     */
    [[nodiscard]] virtual const char *getComponentName() const = 0;

    /**
     * @brief Get a reference to a component at the specified chunk position.
     * @param cPos The position of the chunk.
     * @return A reference to the component at the specified chunk position.
     */
    virtual IComponentRef *getComponentRef(Chunks::chunkPos_t cPos) = 0;

    /**
     * @brief Get a dummy reference to a component at the specified chunk position.
     * @param cPos The position of the chunk.
     * @return A constant reference to a dummy component at the specified chunk position.
     */
    [[nodiscard]] virtual const IComponentRef *getDummyComponentRef(Chunks::chunkPos_t cPos) const = 0;

    /**
     * @brief Get the number of elements in the component pool.
     * @return The number of elements in the pool.
     */
    [[nodiscard]] virtual size_t elemCount() const = 0;

    /**
     * @brief Get the number of chunks in the component pool.
     * @return The number of chunks in the pool.
     */
    [[nodiscard]] virtual size_t chunkCount() const = 0;

    /**
     * @brief Add a chunk to the component pool.
     * @param elemCount The number of elements in the chunk to be added.
     */
    virtual void addChunk(size_t elemCount) = 0;
};

} // namespace ECS::C