/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ChunkPool
*/

#pragma once

#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Chunks/IChunk.hpp"
#include "lib_ecs/Chunks/IChunkPool.hpp"
#include <iostream>
#include <span>
#include <vector>

namespace ECS::Chunks {
/**
 * @brief Abstract base class for a pool of chunks.
 *
 * @tparam T The type of elements stored in the chunks.
 */
template<typename T>
class AChunkPool : public IChunkPool<T> {
public:
    /**
     * @brief Default constructor.
     */
    AChunkPool() = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~AChunkPool() {
        // no need to delete chunks, they are managed by the pool
    };

    /**
     * @brief Access an element by its position.
     *
     * @param pos The position of the element.
     * @return T* Pointer to the element.
     */
    T *getElem(chunkPos_t pos) override
    {
        return _chunks[std::get<0>(pos)]->getElem(std::get<1>(pos));
    }

    /**
     * @brief Access an element by its position (const version).
     *
     * @param pos The position of the element.
     * @return const T* Pointer to the element.
     */
    const T *getElem(chunkPos_t pos) const override
    {
        return _chunks[std::get<0>(pos)]->getElem(std::get<1>(pos));
    }

    /**
     * @brief Access a chunk by index.
     *
     * @param index The index of the chunk.
     * @return IChunk<T>* Pointer to the chunk.
     */
    IChunk<T> *getChunk(size_t index) override { return _chunks[index]; }

    /**
     * @brief Access a chunk by index (const version).
     *
     * @param index The index of the chunk.
     * @return const IChunk<T>* Reference to the chunk.
     */
    const IChunk<T> *getChunk(size_t index) const override { return _chunks[index]; }

    /**
     * @brief get all chunks
     *
     * @return std::vector<IChunk<T> *> Vector of references to all chunks.
     */
    const std::vector<IChunk<T> *> &getChunks() override { return _chunks; }

    // same but with span
    std::span<IChunk<T> *> getChunksSpan() override { return _chunks; }

    // same but with range
    std::ranges::input_range auto &&getChunksRange() { return _chunks; }

    /**
     * @brief set values at indexes
     *
     * @param indexes The indexes of the elements.
     */
    void setValueAtIndexes(const cPosArr_t &indexes, const T &value) override
    {
        for (const auto &index : indexes) {
            *(_chunks[std::get<0>(index)]->getElem(std::get<1>(index))) = value;
        }
    }

    // same but with span
    void setValueAtIndexes(std::span<chunkPos_t> indexes, const T &value) override
    {
        for (const auto &index : indexes) {
            *(_chunks[std::get<0>(index)]->getElem(std::get<1>(index))) = value;
        }
    }

    // same but with range
    void setValueAtIndexes(std::ranges::input_range auto &&indexes, const T &value)
    {
        for (const auto &index : indexes) {
            *(_chunks[std::get<0>(index)]->getElem(std::get<1>(index))) = value;
        }
    }

    void setValuesAtIndexes(const cPosArr_t &indexes, const std::vector<T> &values) override
    {
        for (size_t i = 0; i < indexes.size(); i++) {
            *(_chunks[std::get<0>(indexes[i])]->getElem(std::get<1>(indexes[i]))) = values[i];
        }
    }

    // same but with span
    void setValuesAtIndexes(std::span<chunkPos_t> indexes, const std::span<T> values) override
    {
        for (size_t i = 0; i < indexes.size(); i++) {
            *(_chunks[std::get<0>(indexes[i])]->getElem(std::get<1>(indexes[i]))) = values[i];
        }
    }

    // same but with range
    void setValuesAtIndexes(std::ranges::input_range auto &&indexes, const std::vector<T> &values)
    {

        size_t i = 0;
        for (const auto &index : indexes) {
            *(_chunks[std::get<0>(index)]->getElem(std::get<1>(index))) = values[i];
            i++;
        }
    }

    template<std::size_t Idx, typename... Ts>
    void getValuesAtIndexes(const cPosArr_t &indexes, std::vector<std::tuple<Ts...>> &values)
    {
        for (size_t i = 0; i < indexes.size(); i++) {
            std::get<Idx>(values[i]) =
                *(_chunks[std::get<0>(indexes[i])]->getElem(std::get<1>(indexes[i])));
        }
    }

    // same but with span
    template<std::size_t Idx, typename... Ts>
    void getValuesAtIndexes(std::span<chunkPos_t> indexes, std::vector<std::tuple<Ts...>> &values)
    {
        for (size_t i = 0; i < indexes.size(); i++) {
            std::get<Idx>(values[i]) =
                *(_chunks[std::get<0>(indexes[i])]->getElem(std::get<1>(indexes[i])));
        }
    }

    // same but with range
    template<std::size_t Idx, typename... Ts>
    void getValuesAtIndexes(
        std::ranges::input_range auto &&indexes, std::vector<std::tuple<Ts...>> &values
    )
    {
        size_t i = 0;
        for (const auto &index : indexes) {
            std::get<Idx>(values[i]) = *(_chunks[std::get<0>(index)]->getElem(std::get<1>(index)));
            i++;
        }
    }

    /**
     * @brief Get the total number of elements in all chunks.
     *
     * @return chunk_pos_t The total number of elements.
     */
    [[nodiscard]] chunk_idx_t elemCount() const override
    {
        chunk_idx_t size = 0;
        for (size_t i = 0; i < _chunks.size(); i++) {
            size += _chunks[i]->elemCount();
        }
        return size;
    }

    /**
     * @brief Get the total number of chunks.
     *
     * @return chunk_pos_t The total number of chunks.
     */
    [[nodiscard]] chunk_idx_t chunkCount() const override { return _chunks.size(); }

    /**
     * @brief Add a new chunk with a specified number of elements.
     *
     * @param elemCount The number of elements in the new chunk.
     */
    void addChunk(size_t elemCount) override = 0;

protected:
    std::vector<IChunk<T> *> _chunks {}; ///< Vector of chunks.
};
} // namespace ECS::Chunks
