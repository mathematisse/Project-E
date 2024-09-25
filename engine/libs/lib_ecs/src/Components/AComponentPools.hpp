/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AComponentPool
*/

#pragma once

#include "Chunks/IChunkPool.hpp"
#include "Components/IComponentPool.hpp"
#include "Components/Components.hpp"
#include "Chunks/StandardChunkPool.hpp"
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace ECS::Components
{
    template <typename T>
    class AComponentPool : virtual public IComponentPool
    {
    public:
        explicit AComponentPool(std::string componentName) : _x(), _componentName(std::move(componentName)) {}
        ~AComponentPool() override = default;
        IComponent* operator[](Chunks::ChunkPos cPos) override
        {
            return Components::Component<T>(this->_x.operator[](cPos));
        }
        const IComponent* operator[](Chunks::ChunkPos cPos) const override
        {
            return Components::Component<T>(this->_x.operator[](cPos));
        }
        [[nodiscard]] uint64_t elemCount() const override { return this->_x.elemCount(); }
        [[nodiscard]] uint64_t chunkCount() const override { return this->_x.chunkCount(); }
        [[nodiscard]] const std::string &getComponentName() const override { return this->_componentName; }
        void addChunk(size_t elemCount) override
        {
            std::cout << "Adding chunk with " << elemCount << " elements\nFor component " << this->_componentName << std::endl;
            this->_x.addChunk(elemCount);
        }
        std::vector<Chunks::IChunkPool<T> *> getChunkPools()
        {
            return {this->_x.get()};
        }

    protected:
        Chunks::IChunkPool<T> _x;
        std::string _componentName;
    };

    template <typename T>
    class AComponentPool2 : public AComponentPool<T>
    {
    public:
        explicit AComponentPool2(const std::string &componentName) : AComponentPool<T>(componentName), _y() {}
        IComponent* operator[](Chunks::ChunkPos cPos) override
        {
            std::cout << "Getting component of type " << this->_componentName << " at " << cPos.chunkIndex << " and " << cPos.elemIndex << std::endl;
            std::cout << "Sizes: " << this->_x->getChunks().size() << " and " << this->_y->getChunks().size() << std::endl;
            return Components::Component2<T>(this->_x->operator[](cPos).get(), this->_y->operator[](cPos).get());
        }
        const IComponent* operator[](Chunks::ChunkPos cPos) const override
        {
            return Components::Component2<T>(this->_x->operator[](cPos).get(), this->_y->operator[](cPos).get());
        }
        void addChunk(size_t elemCount) override
        {
            std::cout << "Adding chunk with " << elemCount << " elements\nWADDIHADUIHADUDAHDIAHADHAUDHIUDHA\nHAUDHIUDHA\nHAUDHIUDHA\nHAUDHIUDHA\nHHDAIUHDUADAUHDIHAUDHIUDHA\n";
            this->_x->addChunk(elemCount);
            this->_y->addChunk(elemCount);
        }
        std::vector<Chunks::IChunkPool<T> *> getChunkPools()
        {
            return {this->_x.get(), this->_y.get()};
        }

    protected:
        Chunks::IChunkPool<T> _y;
    };

    template <typename T>
    class AComponentPool3 : public AComponentPool2<T>
    {
    public:
        explicit AComponentPool3(const std::string &componentName) : AComponentPool2<T>(componentName), _z() {}
        IComponent* operator[](Chunks::ChunkPos cPos) override
        {
            Components::Component3<T> elem(this->_x->operator[](cPos).get(), this->_y->operator[](cPos).get(), this->_z->operator[](cPos).get());
            return Components::Component3<T>(elem);
        }
        const IComponent* operator[](Chunks::ChunkPos cPos) const override
        {
            Components::Component3<T> elem(this->_x->operator[](cPos).get(), this->_y->operator[](cPos).get(), this->_z->operator[](cPos).get());
            return Components::Component3<T>(elem);
        }
        void addChunk(size_t elemCount) override
        {
            this->_x->addChunk(elemCount);
            this->_y->addChunk(elemCount);
            this->_z->addChunk(elemCount);
        }
        std::vector<Chunks::IChunkPool<T> *> getChunkPools()
        {
            return {this->_x.get(), this->_y.get(), this->_z.get()};
        }

    protected:
        Chunks::IChunkPool<T> _z;
    };

    template <typename T>
    class AComponentPool4 : public AComponentPool3<T>
    {
    public:
        explicit AComponentPool4(const std::string &componentName) : AComponentPool3<T>(componentName), _w() {}
        IComponent* operator[](Chunks::ChunkPos cPos) override
        {
            return Components::Component4<T>(this->_x->operator[](cPos).get(), this->_y->operator[](cPos).get(), (this->_z->operator[](cPos).get())), (this->_w->operator[](cPos).get());
        }
        const IComponent* operator[](Chunks::ChunkPos cPos) const override
        {
            return Components::Component4<T>(this->_x->operator[](cPos).get(), this->_y->operator[](cPos).get(), (this->_z->operator[](cPos).get())), (this->_w->operator[](cPos).get());
        }
        void addChunk(size_t elemCount) override
        {
            this->_x->addChunk(elemCount);
            this->_y->addChunk(elemCount);
            this->_z->addChunk(elemCount);
            this->_w->addChunk(elemCount);
        }
        std::vector<Chunks::IChunkPool<T> *> getChunkPools() override
        {
            return {this->_x.get(), this->_y.get(), this->_z.get(), this->_w.get()};
        }

    protected:
        Chunks::IChunkPool<T> _w;
    };
}
