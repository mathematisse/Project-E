/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntityPool
*/

#pragma once

#include "Components/PureComponentPools.hpp"
#include "Entities/IEntityPool.hpp"


namespace ECS::E {
class AEntityPool : public IEntityPool
{
public:
  AEntityPool(std::string entityName, size_t chunkSize);
  ~AEntityPool() override;
  AEntityPool(const AEntityPool &other) = default;
  AEntityPool(AEntityPool &&other) = default;
  AEntityPool &operator=(const AEntityPool &other) = default;
  AEntityPool &operator=(AEntityPool &&other) = default;

  [[nodiscard]] const std::string &getEntityName() const override;
  std::vector<Chunks::ChunkPos> &getFreePos() override;
  Chunks::chunk_pos_t getTotalSize() override;
  Chunks::chunk_pos_t getUsedSize() override;
  [[nodiscard]] Chunks::chunk_pos_t getChunkCount() const override;
  C::IComponentPool *getComponentPool(const std::string &name) override;
  void addChunk() override;

protected:
  std::string _entityName;
  size_t _chunkSize;

  std::vector<Chunks::ChunkPos> _freePos;
  C::EntityStatusPool _entityStatusPool;
  C::ChunkPosPool _chunkPosPool;
};
}// namespace ECS::E
