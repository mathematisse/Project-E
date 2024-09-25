/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base StandardChunks
*/

#pragma once

#include "Chunks/AChunk.hpp"


    namespace ECS::Chunks
    {
        template <typename T>
        class StandardChunk : public AChunk<T>
        {
        public:
            explicit StandardChunk(size_t elemCount)
                : AChunk<T>(STDCHUNK, elemCount)
            {
                this->_data = new T[elemCount];
            }
            ~StandardChunk() override
            {
                this->_data.reset();
            }

            StandardChunk(const StandardChunk &other)
                : AChunk<T>(other)
            {
                this->_data = new T[other._elemCount];
                for (size_t i = 0; i < other._elemCount; i++) {
                    this->_data[i] = other._data[i];
                }
            }
            StandardChunk(StandardChunk &&other) = delete;
            StandardChunk &operator=(const StandardChunk &other) = delete;
            StandardChunk &operator=(StandardChunk &&other) = delete;
            
        };
    }

