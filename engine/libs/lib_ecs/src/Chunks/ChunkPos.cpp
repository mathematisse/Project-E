/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ChunkPos
*/

#include "Chunks/ChunkPos.hpp"


namespace ECS { namespace Chunks {
// redefine comparison operators
bool ChunkPos::operator==(const ChunkPos &rhs) const
{
  return chunkIndex == rhs.chunkIndex && elemIndex == rhs.elemIndex;
}
bool ChunkPos::operator!=(const ChunkPos &rhs) const { return !(*this == rhs); }
bool ChunkPos::operator<(const ChunkPos &rhs) const
{
  return chunkIndex < rhs.chunkIndex || (chunkIndex == rhs.chunkIndex && elemIndex < rhs.elemIndex);
}
bool ChunkPos::operator>(const ChunkPos &rhs) const { return rhs < *this; }
bool ChunkPos::operator<=(const ChunkPos &rhs) const { return !(rhs < *this); }
bool ChunkPos::operator>=(const ChunkPos &rhs) const { return !(*this < rhs); }
} }// namespace ECS::Chunks
