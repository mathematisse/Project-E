/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ChunkPos
*/

#include "lib_ecs/Chunks/ChunkPos.hpp"

namespace ECS {
namespace Chunks {

ChunkPos::ChunkPos():
    chunkIndex(0),
    elemIndex(0)
{
}

ChunkPos::ChunkPos(const chunk_idx_t &chunkIndex, const chunk_idx_t &elemIndex):
    chunkIndex(chunkIndex),
    elemIndex(elemIndex)
{
}

ChunkPos::ChunkPos(const std::tuple<chunk_idx_t, chunk_idx_t> &tup):
    chunkIndex(std::get<0>(tup)),
    elemIndex(std::get<1>(tup))
{
}

// redefine comparison operators
bool ChunkPos::operator==(const ChunkPos &rhs) const
{
    return chunkIndex == rhs.chunkIndex && elemIndex == rhs.elemIndex;
}
bool ChunkPos::operator!=(const ChunkPos &rhs) const { return !(rhs == *this); }
bool ChunkPos::operator<(const ChunkPos &rhs) const
{
    return chunkIndex < rhs.chunkIndex || (chunkIndex == rhs.chunkIndex && elemIndex < rhs.elemIndex);
}
bool ChunkPos::operator>(const ChunkPos &rhs) const { return rhs < *this; }
bool ChunkPos::operator<=(const ChunkPos &rhs) const { return !(rhs < *this); }
bool ChunkPos::operator>=(const ChunkPos &rhs) const { return !(*this < rhs); }

// cast to tuple
ChunkPos::operator std::tuple<chunk_idx_t, chunk_idx_t>() const { return {chunkIndex, elemIndex}; }
} // namespace Chunks
} // namespace ECS
