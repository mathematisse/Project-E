/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base StandardChunkPool
*/

#pragma once


#include "Chunks/AChunkPool.hpp"
#include "Chunks/StandardChunk.hpp"


    namespace ECS::Chunks
    {

        template <typename T>
        class StandardChunkPool : public AChunkPool<T>
        {
        public:
            StandardChunkPool() : AChunkPool<T>()
            {
            }
            ~StandardChunkPool() = default;
            StandardChunkPool(const StandardChunkPool &other) = delete;
            StandardChunkPool(StandardChunkPool &&other) = delete;
            StandardChunkPool &operator=(const StandardChunkPool &other) = delete;
            StandardChunkPool &operator=(StandardChunkPool &&other) = delete;
            void addChunk(size_t elemCount) override
            {
                std::cout << "Adding chunk with " << elemCount << " elements\n";
                this->_chunks.push_back(std::make_unique<StandardChunk<T>>(elemCount));
                // init array
                for (size_t i = 0; i < elemCount; i++) {
                    this->_chunks.back()->GetData()[i] = T();
                }
            }
            std::vector<IChunk<T*>> getChunks() override
            {
                std::vector<IChunk<T*>> chunks;
                for (auto &chunk : this->_chunks) {
                    chunks.push_back(std::make_unique<StandardChunk<T>>(*dynamic_cast<StandardChunk<T> *>(chunk.get())));
                }
                return chunks;
            }
        };
    }

