/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IChunk
*/

#pragma once

#include <cstddef>
#include <memory>

namespace ECS::Chunks
{
    // bitmask enum
    enum ChunkType : unsigned int {
        NONECHUNK = 0,
        STDCHUNK = 1,
        SSECHUNKI = 2,
        SSECHUNK = 4,
        SSECHUNKD = 8,
        AVXCHUNKI = 16,
        AVXCHUNK = 32,
        AVXCHUNKD = 64,
        CUDACHUNK = 128,
        CUDALINKCHUNK = 256,
        SIMDCHUNK = SSECHUNKI | SSECHUNK | SSECHUNKD | AVXCHUNKI | AVXCHUNK | AVXCHUNKD,
        ANYCUDACHUNK = CUDACHUNK | CUDALINKCHUNK
    };

    template <typename T>
    class IChunk
    {
    public:
        virtual T* operator[](size_t idx) = 0;
        virtual const T* operator[](size_t idx) const = 0;
        [[nodiscard]] virtual size_t ElemCount() const = 0;
        virtual T* GetData() = 0;
        [[nodiscard]] virtual ChunkType GetType() const = 0;
    };

}
