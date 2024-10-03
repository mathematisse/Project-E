/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ChunkPool
*/

#pragma once

#include "Chunks/ChunkPos.hpp"
#include "Chunks/IChunk.hpp"
#include <vector>

namespace ECS::Chunks {
/**
 * @brief Interface for a pool of chunks.
 *
 * @tparam T The type of elements stored in the chunks.
 */
template<typename T> class IChunkPool
{
public:
  /**
   * @brief Default constructor.
   */
  IChunkPool() = default;

  /**
   * @brief Virtual destructor.
   */
  virtual ~IChunkPool() = default;

  IChunkPool(const IChunkPool &other) = default;
  IChunkPool(IChunkPool &&other) = default;
  IChunkPool &operator=(const IChunkPool &other) = default;
  IChunkPool &operator=(IChunkPool &&other) = default;

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
   * @brief Get an element by its position.
   *
   * @param pos The position of the element.
   * @return T* Pointer to the element.
   */
  virtual T *getElem(ChunkPos pos) = 0;

  /**
   * @brief Get an element by its position (const version).
   *
   * @param pos The position of the element.
   * @return const T* Const pointer to the element.
   */
  virtual const T *getElem(ChunkPos pos) const = 0;

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
  virtual std::vector<IChunk<T> *> getChunks() = 0;

  /**
   * @brief Get the total number of elements.
   *
   * @return chunk_pos_t The total number of elements.
   */
  [[nodiscard]] virtual chunk_pos_t elemCount() const = 0;

  /**
   * @brief Get the total number of chunks.
   *
   * @return chunk_pos_t The total number of chunks.
   */
  [[nodiscard]] virtual chunk_pos_t chunkCount() const = 0;
};
}// namespace ECS::Chunks
