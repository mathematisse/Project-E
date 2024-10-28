/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ChunkPool
*/

#pragma once

#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Chunks/IChunk.hpp"
#include <span>
#include <vector>

namespace ECS::Chunks {
/**
 * @brief Interface for a pool of chunks.
 *
 * @tparam T The type of elements stored in the chunks.
 */
template<typename T>
class IChunkPool {
public:
    /**
     * @brief Default constructor.
     */
    IChunkPool() = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~IChunkPool() = default;

    /**
     * @brief Get a chunk by index.
     *
     * @param index The index of the chunk.
     * @return IChunk<T>* Reference to the chunk.
     */
    virtual IChunk<T> *getChunk(size_t index) = 0;

    /**
     * @brief Get a chunk by index (const version).
     *
     * @param index The index of the chunk.
     * @return const IChunk<T>* Const pointer to the chunk.
     */
    virtual const IChunk<T> *getChunk(size_t index) const = 0;

    /**
     * @brief set values at indexes
     *
     * @param indexes The indexes of the elements.
     */
    virtual void setValueAtIndexes(const cPosArr_t &indexes, const T &value) = 0;

    virtual void setValuesAtIndexes(const cPosArr_t &indexes, const std::vector<T> &values) = 0;

    /**
     * @brief Get an element by its position.
     *
     * @param pos The position of the element.
     * @return T* Pointer to the element.
     */
    virtual T *getElem(chunkPos_t pos) = 0;

    /**
     * @brief Get an element by its position (const version).
     *
     * @param pos The position of the element.
     * @return const T* Const pointer to the element.
     */
    virtual const T *getElem(chunkPos_t pos) const = 0;

    /**
     * @brief Add a new chunk with a specified number of elements.
     *
     * @param elemCount The number of elements in the new chunk.
     */
    virtual void addChunk(size_t elemCount) = 0;

    /**
     * @brief Get all chunks.
     *
     * @return std::vector<IChunk<T>*> Vector of pointers to all chunks.
     */
    virtual const std::vector<IChunk<T> *> &getChunks() = 0;
    virtual std::span<IChunk<T> *> getChunksSpan() = 0;
    /**
     * @brief Get the total number of elements.
     *
     * @return chunk_pos_t The total number of elements.
     */
    [[nodiscard]] virtual chunk_idx_t elemCount() const = 0;

    /**
     * @brief Get the total number of chunks.
     *
     * @return chunk_pos_t The total number of chunks.
     */
    [[nodiscard]] virtual chunk_idx_t chunkCount() const = 0;

    virtual void setValueAtIndexes(std::span<chunkPos_t> indexes, const T &value) = 0;

    virtual void setValuesAtIndexes(std::span<chunkPos_t> indexes, const std::span<T> values) = 0;
};
} // namespace ECS::Chunks
