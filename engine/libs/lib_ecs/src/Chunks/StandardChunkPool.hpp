/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base StandardChunkPool
*/

#pragma once

#include "Chunks/AChunkPool.hpp"
#include "Chunks/StandardChunk.hpp"

namespace ECS::Chunks {
/**
 * @brief A pool of standard chunks.
 *
 * This class manages a collection of StandardChunk objects.
 *
 * @tparam T The type of elements stored in the chunks.
 */
template<typename T>
class StandardChunkPool : public AChunkPool<T> {
public:
    /**
     * @brief Construct a new StandardChunkPool object.
     *
     * Initializes the base AChunkPool.
     */
    StandardChunkPool():
        AChunkPool<T>()
    {
    }

    /**
     * @brief Destroy the StandardChunkPool object.
     *
     * Default destructor.
     */
    ~StandardChunkPool()
    {
        for (auto &chunk : this->_chunks) {
            delete chunk;
        }
    }

    StandardChunkPool(const StandardChunkPool &other) = default;
    StandardChunkPool(StandardChunkPool &&other) = default;
    StandardChunkPool &operator=(const StandardChunkPool &other) = default;
    StandardChunkPool &operator=(StandardChunkPool &&other) = default;

    /**
     * @brief Add a new chunk to the pool.
     *
     * Creates a new StandardChunk with the specified element count and adds it to the pool.
     *
     * @param elemCount The number of elements in the new chunk.
     */
    void addChunk(size_t elemCount) override { this->_chunks.push_back(new StandardChunk<T>(elemCount)); }
};
} // namespace ECS::Chunks
