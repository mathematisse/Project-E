/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ChunkPool
*/

#pragma once

#include "Chunks/IChunk.hpp"
#include "Chunks/ChunkPos.hpp"
#include "Chunks/IChunkPool.hpp"
#include <vector>

namespace ECS::Chunks
{
    /**
     * @brief Abstract base class for a pool of chunks.
     * 
     * @tparam T The type of elements stored in the chunks.
     */
    template <typename T>
    class AChunkPool : public IChunkPool<T>
    {
    public:
        /**
         * @brief Default constructor.
         */
        AChunkPool() = default;

        /**
         * @brief Virtual destructor.
         */
        virtual ~AChunkPool() = default;
        
        AChunkPool(const AChunkPool &other) = default;
        AChunkPool(AChunkPool &&other) = default;
        AChunkPool &operator=(const AChunkPool &other) = default;
        AChunkPool &operator=(AChunkPool &&other) = default;

        /**
         * @brief Access an element by its position.
         * 
         * @param pos The position of the element.
         * @return T* Pointer to the element.
         */
        T* getElem(ChunkPos pos) override
        {
            return _chunks[pos.chunkIndex]->getElem(pos.elemIndex);
        }

        /**
         * @brief Access an element by its position (const version).
         * 
         * @param pos The position of the element.
         * @return const T* Pointer to the element.
         */
        const T* getElem(ChunkPos pos) const override
        {
            return _chunks[pos.chunkIndex]->getElem(pos.elemIndex);
        }

        /**
         * @brief Access a chunk by index.
         * 
         * @param index The index of the chunk.
         * @return IChunk<T>* Pointer to the chunk.
         */
        IChunk<T> *getChunk(size_t index) override
        {
            return _chunks[index];
        }

        /**
         * @brief Access a chunk by index (const version).
         * 
         * @param index The index of the chunk.
         * @return const IChunk<T>* Reference to the chunk.
         */
        const IChunk<T> *getChunk(size_t index) const override
        {
            return _chunks[index];
        }

        /**
         * @brief get all chunks
         *
         * @return std::vector<IChunk<T> *> Vector of references to all chunks.
         */
        std::vector<IChunk<T> *> getChunks() override
        {
            return _chunks;
        }

        /**
         * @brief Get the total number of elements in all chunks.
         * 
         * @return chunk_pos_t The total number of elements.
         */
        [[nodiscard]] chunk_pos_t elemCount() const override
        {
            chunk_pos_t size = 0;
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
        [[nodiscard]] chunk_pos_t chunkCount() const override
        {
            return _chunks.size();
        }

        /**
         * @brief Add a new chunk with a specified number of elements.
         * 
         * @param elemCount The number of elements in the new chunk.
         */
         void addChunk(size_t elemCount) override = 0;

    protected:
        std::vector<IChunk<T> *> _chunks{}; ///< Vector of chunks.
    };
}
