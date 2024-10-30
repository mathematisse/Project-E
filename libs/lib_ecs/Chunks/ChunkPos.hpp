/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ChunkPos
*/

#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

namespace ECS::Chunks {

using chunk_idx_t = uint64_t;
using elem_idx_t = uint64_t;
using chunkPos_t = std::tuple<chunk_idx_t, elem_idx_t>;
using cPosArr_t = std::vector<chunkPos_t>;

/**
 * @brief A struct representing a position in a chunk pool.
 */
struct ChunkPos {
    // constructor from tuple
    ChunkPos();
    ChunkPos(const chunk_idx_t &chunkIndex, const chunk_idx_t &elemIndex);
    explicit ChunkPos(const std::tuple<chunk_idx_t, chunk_idx_t> &tup);

    chunk_idx_t chunkIndex;
    chunk_idx_t elemIndex;

    // redefine comparison operators
    bool operator==(const ChunkPos &rhs) const;
    bool operator!=(const ChunkPos &rhs) const;
    bool operator<(const ChunkPos &rhs) const;
    bool operator>(const ChunkPos &rhs) const;
    bool operator<=(const ChunkPos &rhs) const;
    bool operator>=(const ChunkPos &rhs) const;

    // cast to tuple
    operator std::tuple<chunk_idx_t, chunk_idx_t>() const;
};

} // namespace ECS::Chunks
