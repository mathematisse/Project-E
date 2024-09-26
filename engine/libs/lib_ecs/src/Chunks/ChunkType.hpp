/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IChunk
*/

#pragma once

namespace ECS::Chunks
{
    /**
     * @brief Enum representing different types of chunks.
     */
    enum ChunkType : unsigned int {
        NONECHUNK = 0,       ///< No chunk type.
        STDCHUNK = 1,        ///< Standard chunk.
        SSECHUNKI = 2,       ///< SSE chunk integer.
        SSECHUNK = 4,        ///< SSE chunk.
        SSECHUNKD = 8,       ///< SSE chunk double.
        AVXCHUNKI = 16,      ///< AVX chunk integer.
        AVXCHUNK = 32,       ///< AVX chunk.
        AVXCHUNKD = 64,      ///< AVX chunk double.
        CUDACHUNK = 128,     ///< CUDA chunk.
        CUDALINKCHUNK = 256, ///< CUDA link chunk.
        SIMDCHUNK = SSECHUNKI | SSECHUNK | SSECHUNKD | AVXCHUNKI | AVXCHUNK | AVXCHUNKD, ///< SIMD chunk.
        ANYCUDACHUNK = CUDACHUNK | CUDALINKCHUNK ///< Any CUDA chunk.
    };

}
