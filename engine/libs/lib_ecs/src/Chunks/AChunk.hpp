/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AChunk
*/

#pragma once

#include "Chunks/IChunk.hpp"
#include <memory>

namespace ECS::Chunks
{
    template <typename T>
    class AChunk : public IChunk<T>
    {
    public:
        virtual ~AChunk() = default;

        AChunk(const AChunk &other) : _type{other._type}, _elemCount{other._elemCount}, _data{new T[other._elemCount]}
        {
            for (size_t i = 0; i < other._elemCount; i++) {
                _data[i] = other._data[i];
            }
        }
        AChunk(AChunk &&other) = delete;
        AChunk &operator=(const AChunk &other) = delete;
        AChunk &operator=(AChunk &&other) = delete;

        AChunk(ChunkType type, size_t elemCount) : _type(type), _elemCount{elemCount}, _data{new T[elemCount]} {}
        
        T*operator[](size_t index) override
        {
            return new T(_data[index]);
        }
        
        const T*operator[](size_t index) const override
        {
            return new T(_data[index]);
        }
        
        [[nodiscard]] size_t ElemCount() const override { return _elemCount; }
        
        T *GetData() override { return _data.get(); }
        
        [[nodiscard]] ChunkType GetType() const override { return _type; }

    protected:
        ChunkType _type;
        size_t _elemCount;
        T _data[];
    };
}
