/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base StandardChunkPool
*/

#pragma once

#include "lib_ecs/Chunks/AChunkPool.hpp"
#include "lib_ecs/Chunks/StandardChunk.hpp"

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
        // for (auto *chunk : this->_chunks) {
        //     delete chunk;
        // }
    }

    /**
     * @brief Add a new chunk to the pool.
     *
     * Creates a new StandardChunk with the specified element count and adds it to the pool.
     *
     * @param elemCount The number of elements in the new chunk.
     */
    void addChunk(size_t elemCount) override
    {
        this->_chunks.push_back(new StandardChunk<T>(elemCount));
    }

    void deleteEverything() override
    {
        for (auto *chunk : this->_chunks) {
            delete chunk;
        }
        this->_chunks.clear();
    }

    void resetAtChunkPos(Chunks::chunkPos_t cPos) override
    {
        this->_chunks[std::get<0>(cPos)]->resetAtIndex(std::get<1>(cPos));
    }
};
} // namespace ECS::Chunks
