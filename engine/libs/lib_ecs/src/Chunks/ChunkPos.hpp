/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ChunkPos
*/

#pragma once

#include <cstdint>


    namespace ECS::Chunks
    {
        
        using chunk_pos_t = uint64_t;
        
        /**
         * @brief A struct representing a position in a chunk pool.
         */
        struct ChunkPos {
            chunk_pos_t chunkIndex;
            chunk_pos_t elemIndex;

            // redefine comparison operators
            bool operator==(const ChunkPos &rhs) const;
            bool operator!=(const ChunkPos &rhs) const;
            bool operator<(const ChunkPos &rhs) const;
            bool operator>(const ChunkPos &rhs) const;
            bool operator<=(const ChunkPos &rhs) const;
            bool operator>=(const ChunkPos &rhs) const;
        };
    }

