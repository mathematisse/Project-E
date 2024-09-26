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
        /**
         * @brief A struct representing a position in a chunk pool.
         */
        struct ChunkPos {
            uint64_t chunkIndex;
            uint64_t elemIndex;

            // redefine comparison operators
            bool operator==(const ChunkPos &rhs) const;
            bool operator!=(const ChunkPos &rhs) const;
            bool operator<(const ChunkPos &rhs) const;
            bool operator>(const ChunkPos &rhs) const;
            bool operator<=(const ChunkPos &rhs) const;
            bool operator>=(const ChunkPos &rhs) const;
        };
    }

