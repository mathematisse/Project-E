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
#include <vector>

namespace ECS::Components
{

    /**
     * @class AComponentPool
     * @brief A template class for managing component pools.
     * 
     * This class provides an interface for managing pools of components of type T.
     * It inherits from the IComponentPool interface and provides implementations
     * for various component pool operations.
     * 
     * @tparam T The type of components managed by the pool.
     */
    template <typename T>
    class AComponentPool : virtual public IComponentPool
    {
    public:
        /**
         * @brief Constructs an AComponentPool with a given component name.
         * 
         * @param componentName The name of the component.
         */
        explicit AComponentPool(std::string componentName) : IComponentPool(), _x(), _componentName(std::move(componentName)) {}

        /**
         * @brief Destructor for AComponentPool.
         */
        ~AComponentPool() override;

        AComponentPool(const AComponentPool &other) = default;
        AComponentPool &operator=(const AComponentPool &other) = default;
        AComponentPool(AComponentPool &&other) = default;
        AComponentPool &operator=(AComponentPool &&other) = default;

        /**
         * @brief Retrieves a reference to a component at a given chunk position.
         * 
         * @param cPos The position of the chunk.
         * @return IComponentRef A reference to the component.
         */
        IComponentRef getComponentRef(Chunks::ChunkPos cPos) override {
            return Components::ComponentRef<T>(this->_x.getElem(cPos));
        }

        /**
         * @brief Retrieves a dummy reference to a component at a given chunk position.
         * 
         * @param cPos The position of the chunk.
         * @return IComponentRef A dummy reference to the component.
         */
        [[nodiscard]] IComponentRef getDummyComponentRef(Chunks::ChunkPos cPos) const override {
            return Components::ComponentRef<T>(*(this->_x.getElem(cPos)));
        }

        /**
         * @brief Gets the number of elements in the pool.
         * 
         * @return uint64_t The number of elements.
         */
        [[nodiscard]] uint64_t elemCount() const override;

        /**
         * @brief Gets the number of chunks in the pool.
         * 
         * @return uint64_t The number of chunks.
         */
        [[nodiscard]] uint64_t chunkCount() const override;

        /**
         * @brief Gets the name of the component.
         * 
         * @return const std::string& The name of the component.
         */
        [[nodiscard]] const std::string &getComponentName() const override;

        /**
         * @brief Adds a chunk to the pool with a specified number of elements.
         * 
         * @param elemCount The number of elements in the chunk.
         */
        void addChunk(size_t elemCount) override;

        /**
         * @brief Retrieves the chunk pools managed by this component pool.
         * 
         * @return std::vector<Chunks::IChunkPool<T>*> A vector of chunk pools.
         */
        std::vector<Chunks::IChunkPool<T> *> getChunkPools();

    protected:
        Chunks::StandardChunkPool<T> _x; ///< The standard chunk pool for components of type T.
        std::string _componentName; ///< The name of the component.
    };

    /**
     * @class AComponentPool2
     * @brief A specialized component pool class template.
     * 
     * This class template extends the AComponentPool class template and provides
     * additional functionality for managing component pools.
     * 
     * @tparam T The type of components managed by this pool.
     */
    template <typename T>
    class AComponentPool2 : public AComponentPool<T>
    {
    public:
            /**
             * @brief Constructs a new AComponentPool2 object.
             * 
             * @param componentName The name of the component.
             */
            explicit AComponentPool2(const std::string &componentName);

            /**
             * @brief Retrieves a reference to a component at the specified chunk position.
             * 
             * @param cPos The position of the chunk.
             * @return IComponentRef A reference to the component.
             */
            IComponentRef getComponentRef(Chunks::ChunkPos cPos) override {
                return Components::ComponentRef2<T>(this->_x.getElem(cPos), this->_y.getElem(cPos));
            }

            /**
             * @brief Retrieves a dummy reference to a component at the specified chunk position.
             * 
             * @param cPos The position of the chunk.
             * @return IComponentRef A dummy reference to the component.
             */
            [[nodiscard]] IComponentRef getDummyComponentRef(Chunks::ChunkPos cPos) const override {
                return Components::ComponentRef2<T>(*(this->_x.getElem(cPos)), *(this->_y.getElem(cPos)));
            }

            /**
             * @brief Adds a new chunk to the component pool.
             * 
             * @param elemCount The number of elements in the new chunk.
             */
            void addChunk(size_t elemCount) override;

            /**
             * @brief Retrieves the chunk pools managed by this component pool.
             * 
             * @return std::vector<Chunks::IChunkPool<T> *> A vector of pointers to the chunk pools.
             */
            std::vector<Chunks::IChunkPool<T> *> getChunkPools();

    protected:
            Chunks::StandardChunkPool<T> _y; ///< A secondary chunk pool for managing components.
    };


    /**
     * @class AComponentPool3
     * @brief A specialized component pool class that extends AComponentPool2 to manage three-dimensional component data.
     * 
     * @tparam T The type of component managed by this pool.
     * 
     * This class provides functionality to manage components in a three-dimensional space, 
     * extending the capabilities of AComponentPool2. It includes methods to get component 
     * references, add chunks of components, and retrieve chunk pools.
     */
    template <typename T>
    class AComponentPool3 : public AComponentPool2<T>
    {
    public:
     
    /**
     * @brief Constructs a new AComponentPool3 object.
     * 
     * @param componentName The name of the component managed by this pool.
     */
    explicit AComponentPool3(const std::string &componentName) : AComponentPool2<T>(componentName), _z() {}
     
    /**
     * @brief Retrieves a reference to the component at the specified chunk position.
     * 
     * @param cPos The position of the chunk.
     * @return IComponentRef A reference to the component at the specified chunk position.
     */
        IComponentRef getComponentRef(Chunks::ChunkPos cPos) override
        {
            return Components::ComponentRef3<T>(this->_x.getElem(cPos), this->_y.getElem(cPos), this->_z.getElem(cPos));
        }
     
    /**
     * @brief Retrieves a constant reference to a dummy component at the specified chunk position.
     * 
     * @param cPos The position of the chunk.
     * @return IComponentRef A constant reference to a dummy component at the specified chunk position.
     */
        [[nodiscard]] IComponentRef getDummyComponentRef(Chunks::ChunkPos cPos) const override
        {
            return Components::ComponentRef3<T>(*(this->_x.getElem(cPos)), *(this->_y.getElem(cPos)), *(this->_z.getElem(cPos)));
        }
     
    /**
     * @brief Adds a new chunk of components to the pool.
     * 
     * @param elemCount The number of elements in the new chunk.
     */
        void addChunk(size_t elemCount) override
        {
            this->_x->addChunk(elemCount);
            this->_y->addChunk(elemCount);
            this->_z->addChunk(elemCount);
        }
     
    /**
     * @brief Retrieves the chunk pools managed by this component pool.
     * 
     * @return std::vector<Chunks::IChunkPool<T> *> A vector containing pointers to the chunk pools.
     */
        std::vector<Chunks::IChunkPool<T> *> getChunkPools()
        {
            return {this->_x, this->_y, this->_z};
        }

    protected:
        Chunks::StandardChunkPool<T> _z; ///< A third chunk pool for managing components.
    };

     /**
      * @class AComponentPool4
      * @brief A component pool class template that manages four component pools.
      *
      * The AComponentPool4 class template extends the AComponentPool3 class template by adding an additional
      * component pool (_w). It provides methods to get component references, add chunks, and retrieve chunk pools.
      *
      * @tparam T The type of the components managed by the pool.
      */
    template <typename T>
    class AComponentPool4 : public AComponentPool3<T>
    {
    public:
            /**
             * @brief Constructs an AComponentPool4 with the specified component name.
             *
             * @param componentName The name of the component.
             */
            explicit AComponentPool4(const std::string &componentName);

            /**
             * @brief Gets a reference to the component at the specified chunk position.
             *
             * @param cPos The chunk position.
             * @return A reference to the component.
             */
            IComponentRef getComponentRef(Chunks::ChunkPos cPos) override {
                return Components::ComponentRef4<T>(this->_x.getElem(cPos), this->_y.getElem(cPos), this->_z.getElem(cPos), this->_w.getElem(cPos));
            }

            /**
             * @brief Gets a dummy reference to the component at the specified chunk position.
             *
             * @param cPos The chunk position.
             * @return A dummy reference to the component.
             */
            [[nodiscard]] IComponentRef getDummyComponentRef(Chunks::ChunkPos cPos) const override {
                return Components::ComponentRef4<T>(*(this->_x.getElem(cPos)), *(this->_y.getElem(cPos)), *(this->_z.getElem(cPos)), *(this->_w.getElem(cPos)));
            }

            /**
             * @brief Adds a chunk with the specified number of elements to the component pools.
             *
             * @param elemCount The number of elements in the chunk.
             */
            void addChunk(size_t elemCount) override;

            /**
             * @brief Gets the chunk pools managed by this component pool.
             *
             * @return A vector of pointers to the chunk pools.
             */
            std::vector<Chunks::IChunkPool<T> *> getChunkPools() override;

    protected:
            Chunks::StandardChunkPool<T> _w; ///< The fourth component pool.
    };

}
