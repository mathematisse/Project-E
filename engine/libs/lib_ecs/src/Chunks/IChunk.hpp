/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IChunk
*/

#pragma once

#include <cstddef>

#include "Chunks/ChunkType.hpp"

namespace ECS::Chunks
{

    /**
     * @brief Interface for a chunk.
     * 
     * @tparam T Type of elements stored in the chunk.
     * @tparam TContainer Type of container used to store elements.
     */
    template <typename T>
    class IChunk
    {
    public:
        /**
         * @brief Destructor.
         */
        virtual ~IChunk() = default;

        IChunk(const IChunk &other) = delete;
        IChunk(IChunk &&other) = delete;
        IChunk &operator=(const IChunk &other) = delete;
        IChunk &operator=(IChunk &&other) = delete;

        /**
         * @brief Get the element at the specified index.
         * 
         * @param idx Index of the element.
         * @return T& Reference to the element.
         */
        virtual T& getElem(size_t idx) = 0;

        /**
         * @brief Get the element at the specified index (const version).
         * 
         * @param idx Index of the element.
         * @return const T& Const reference to the element.
         */
        virtual const T& getElem(size_t idx) const = 0;

        /**
         * @brief Get the count of elements.
         * 
         * @return size_t Number of elements.
         */
        [[nodiscard]] virtual size_t elemCount() const = 0;

        /**
         * @brief Get the type of the chunk.
         * 
         * @return ChunkType Type of the chunk.
         */
        [[nodiscard]] virtual ChunkType getType() const = 0;
        
    };

}
