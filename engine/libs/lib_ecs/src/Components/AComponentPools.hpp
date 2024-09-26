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
            return new Components::Component<T>(this->_x[cPos]);
        }
        const IComponent* operator[](Chunks::ChunkPos cPos) const override
        {
            return new Components::Component<T>(*(this->_x[cPos]));
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
            return {this->_x};
        }

    protected:
        Chunks::StandardChunkPool<T> _x;
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
            return new Components::Component2<T>(this->_x->operator[](cPos), this->_y->operator[](cPos));
        }
        const IComponent* operator[](Chunks::ChunkPos cPos) const override
        {
            return new Components::Component2<T>(this->_x->operator[](cPos), this->_y->operator[](cPos));
        }
        void addChunk(size_t elemCount) override
        {
            std::cout << "Adding chunk with " << elemCount << " elements\nWADDIHADUIHADUDAHDIAHADHAUDHIUDHA\nHAUDHIUDHA\nHAUDHIUDHA\nHAUDHIUDHA\nHHDAIUHDUADAUHDIHAUDHIUDHA\n";
            this->_x->addChunk(elemCount);
            this->_y->addChunk(elemCount);
        }
        std::vector<Chunks::IChunkPool<T> *> getChunkPools()
        {
            return {this->_x, this->_y};
        }

    protected:
        Chunks::StandardChunkPool<T> _y;
    };

    template <typename T>
    class AComponentPool3 : public AComponentPool2<T>
    {
    public:
        explicit AComponentPool3(const std::string &componentName) : AComponentPool2<T>(componentName), _z() {}
        IComponent* operator[](Chunks::ChunkPos cPos) override
        {
            return new Components::Component3<T>(this->_x->operator[](cPos), this->_y->operator[](cPos), this->_z->operator[](cPos));
        }
        const IComponent* operator[](Chunks::ChunkPos cPos) const override
        {
            return new Components::Component3<T>(this->_x->operator[](cPos), this->_y->operator[](cPos), this->_z->operator[](cPos));
        }
        void addChunk(size_t elemCount) override
        {
            this->_x->addChunk(elemCount);
            this->_y->addChunk(elemCount);
            this->_z->addChunk(elemCount);
        }
        std::vector<Chunks::IChunkPool<T> *> getChunkPools()
        {
            return {this->_x, this->_y, this->_z};
        }

    protected:
        Chunks::StandardChunkPool<T> _z;
    };

    template <typename T>
    class AComponentPool4 : public AComponentPool3<T>
    {
    public:
        explicit AComponentPool4(const std::string &componentName) : AComponentPool3<T>(componentName), _w() {}
        IComponent* operator[](Chunks::ChunkPos cPos) override
        {
            return new Components::Component4<T>(this->_x->operator[](cPos), this->_y->operator[](cPos), (this->_z->operator[](cPos))), (this->_w->operator[](cPos));
        }
        const IComponent* operator[](Chunks::ChunkPos cPos) const override
        {
            return new Components::Component4<T>(this->_x->operator[](cPos), this->_y->operator[](cPos), (this->_z->operator[](cPos))), (this->_w->operator[](cPos));
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
            return {this->_x, this->_y, this->_z, this->_w};
        }

    protected:
        Chunks::StandardChunkPool<T> _w;
    };

    template <typename T, typename... Ts>
    class AComponentPoolVar : public IComponentPool
    {
    public:
        explicit AComponentPoolVar(std::string componentName)
            : _componentName(std::move(componentName)), _pools(std::make_tuple(Chunks::StandardChunkPool<Ts>()...)) {}

        ~AComponentPoolVar() override = default;

        IComponent* operator[](Chunks::ChunkPos cPos) override
        {
            return createComponent(cPos, std::index_sequence_for<Ts...>{});
        }

        const IComponent* operator[](Chunks::ChunkPos cPos) const override
        {
            return createComponent(cPos, std::index_sequence_for<Ts...>{});
        }

        [[nodiscard]] uint64_t elemCount() const override
        {
            return std::get<0>(_pools).elemCount();
        }

        [[nodiscard]] uint64_t chunkCount() const override
        {
            return std::get<0>(_pools).chunkCount();
        }

        [[nodiscard]] const std::string &getComponentName() const override
        {
            return this->_componentName;
        }

        void addChunk(size_t elemCount) override
        {
            addChunkToPools(elemCount, std::index_sequence_for<Ts...>{});
        }

        std::vector<Chunks::IChunkPool<T> *> getChunkPools() override
        {
            return getChunkPools(std::index_sequence_for<Ts...>{});
        }

    protected:
        template <std::size_t... Is>
        IComponent* createComponent(Chunks::ChunkPos cPos, std::index_sequence<Is...>) const
        {
            return new Components::Component<T>(std::get<Is>(_pools)[cPos]...);
        }

        template <std::size_t... Is>
        void addChunkToPools(size_t elemCount, std::index_sequence<Is...>)
        {
            (std::get<Is>(_pools).addChunk(elemCount), ...);
        }

        template <std::size_t... Is>
        std::vector<Chunks::IChunkPool<T> *> getChunkPools(std::index_sequence<Is...>)
        {
            return {&std::get<Is>(_pools)...};
        }

        std::tuple<Chunks::StandardChunkPool<Ts>...> _pools;
        std::string _componentName;
    };
}
