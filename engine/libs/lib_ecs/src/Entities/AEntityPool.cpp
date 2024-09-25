/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AEntityPool
*/

#include "Entities/AEntityPool.hpp"

#include <utility>
#include "Chunks/ChunkPos.hpp"


    namespace ECS::Entities
    {
        AEntityPool::AEntityPool(std::string entityName, size_t chunkSize)
            : _entityName(std::move(entityName)), _chunkSize(chunkSize)
        {
        }

        AEntityPool::~AEntityPool()
        {
        }

        const std::string &AEntityPool::getEntityName() const
        {
            return _entityName;
        }

        std::list<Chunks::ChunkPos> &AEntityPool::getFreePos()
        {
            return _freePos;
        }

        uint64_t AEntityPool::getTotalSize()
        {
            return _entityStatusPool.elemCount();
        }

        uint64_t AEntityPool::getUsedSize()
        {
            return _entityStatusPool.elemCount() - _freePos.size();
        }

        uint64_t AEntityPool::getChunkCount() const
        {
            return _entityStatusPool.chunkCount();
        }

        Components::IComponentPool *AEntityPool::getComponentPool(const std::string &name)
        {
            for (auto &componentPool : getComponentPools()) {
                if (componentPool->getComponentName() == name) {
                    return componentPool;
                }
            }
            return nullptr;
        }

        void AEntityPool::addChunk()
        {
            std::cout << "Adding chunk to entity pool: " << getEntityName() << '\n';
            Chunks::ChunkPos cPos = {getChunkCount(), 0};
            for (auto &componentPool : getComponentPools()) {
                std::cout << "Adding chunk to component pool: " << componentPool->getComponentName() << '\n';
                componentPool->addChunk(_chunkSize);
            }
            // adds all the free positions of the new chunk
            for (size_t i = 0; i < _chunkSize; i++) {
                std::cout << "Adding free pos: " << cPos.chunkIndex << " " << cPos.elemIndex << '\n';
                _freePos.push_back(cPos);
                cPos.elemIndex++;
            }
        }
    }

