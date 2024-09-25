/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IComponentPool
*/

#pragma once

#include "Components/IComponent.hpp"
#include "Chunks/ChunkPos.hpp"
#include <cstddef>
#include <string>
#include <memory>


    namespace ECS::Components
    {
        class IComponentPool
        {
        public:
            IComponentPool() = default;
            virtual ~IComponentPool() = default;
            IComponentPool(const IComponentPool &other) = delete;
            IComponentPool(IComponentPool &&other) = delete;
            IComponentPool &operator=(const IComponentPool &other) = delete;
            IComponentPool &operator=(IComponentPool &&other) = delete;
            [[nodiscard]] virtual const std::string &getComponentName() const = 0;
            virtual IComponent* operator[](Chunks::ChunkPos cPos) = 0;
            virtual const IComponent* operator[](Chunks::ChunkPos cPos) const = 0;
            [[nodiscard]] virtual size_t elemCount() const = 0;
            [[nodiscard]] virtual size_t chunkCount() const = 0;
            virtual void addChunk(size_t elemCount) = 0;
        };
    }

