/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AChunk
*/

#pragma once

#include "Chunks/IChunk.hpp"

namespace ECS::Chunks
{
    template <typename T>
    class AChunk : public IChunk<T>
    {
    public:

        /**
         * @brief Constructor.
         * 
         * @param type Type of the chunk.
         * @param elemCount Number of elements.
         */
        AChunk(ChunkType type, size_t elemCount) : _type(type), _elemCount(elemCount) {}

        /**
         * @brief Destructor.
         */
        virtual ~AChunk() = default;

        AChunk(const AChunk &other) = delete;
        AChunk(AChunk &&other) = delete;
        AChunk &operator=(const AChunk &other) = delete;
        AChunk &operator=(AChunk &&other) = delete;
        
        /**
         * @brief Get the count of elements.
         * 
         * @return size_t Number of elements.
         */
        [[nodiscard]] size_t elemCount() const override { return _elemCount; }
        
        /**
         * @brief Get the type of the chunk.
         * 
         * @return ChunkType Type of the chunk.
         */
        [[nodiscard]] ChunkType getType() const override { return _type; }

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

    protected:
        ChunkType _type;
        size_t _elemCount;
    };
}
