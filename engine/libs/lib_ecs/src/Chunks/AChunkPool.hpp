/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ChunkPool
*/

#pragma once

#include "Chunks/IChunk.hpp"
#include "Chunks/ChunkPos.hpp"
#include "Chunks/IChunkPool.hpp"
#include <vector>
#include <memory>
#include <iostream>

    namespace ECS::Chunks
    {

        template <typename T>
        class AChunkPool : public IChunkPool<T>
        {
        public:
            AChunkPool() = default;
            virtual ~AChunkPool() = default;

            AChunkPool(const AChunkPool &other) = default;
            AChunkPool(AChunkPool &&other) = delete;
            AChunkPool &operator=(const AChunkPool &other) = delete;
            AChunkPool &operator=(AChunkPool &&other) = delete;

            IChunk<T> *operator[](size_t index) override
            {
                return _chunks[index].get();
            }
            T* operator[](ChunkPos pos) override
            {
                std::cout << "Adress of this: " << this << std::endl;
                std::cout << "Getting chunk at " << pos.chunkIndex << " and elem at " << pos.elemIndex << std::endl;
                std::cout << "Adress: " << _chunks[pos.chunkIndex].get() << std::endl;
                std::cout << "Chunk size: " << _chunks[pos.chunkIndex]->ElemCount() << std::endl;
                std::cout << "Elem: " << _chunks[pos.chunkIndex]->GetData()[pos.elemIndex] << std::endl;
                std::cout << "Elem: " << _chunks[pos.chunkIndex]->GetData()[pos.elemIndex] << std::endl;
                T elem = _chunks[pos.chunkIndex]->GetData()[pos.elemIndex];
                return new T(elem);
            }
            const IChunk<T> *operator[](size_t index) const override
            {
                return _chunks[index].get();
            }
            const T* operator[](ChunkPos pos) const override
            {
                T elem = _chunks[pos.chunkIndex]->GetData()[pos.elemIndex];
                return new T(elem);
            }
            [[nodiscard]] uint64_t elemCount() const override
            {
                uint64_t size = 0;
                for (size_t i = 0; i < _chunks.size(); i++) {
                    size += _chunks[i]->ElemCount();
                }
                return size;
            }
            [[nodiscard]] uint64_t chunkCount() const override
            {
                return _chunks.size();
            }

        protected:
            std::vector<IChunk<T*>> _chunks{};
        };
    }

