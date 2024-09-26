/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base StandardChunks
*/

#pragma once

#include "Chunks/AChunk.hpp"
#include <vector>


    namespace ECS::Chunks
    {
        /**
         * @class StandardChunk
         * @brief A standard chunk implementation for storing elements.
         * 
         * This class template inherits from AChunk and provides a standard
         * implementation for storing elements in a vector.
         * 
         * @tparam T The type of elements stored in the chunk.
         */
        template <typename T>
        class StandardChunk : public AChunk<T>
        {
        public:
            /**
             * @brief Constructs a StandardChunk with a specified number of elements.
             * 
             * @param elemCount The number of elements to be stored in the chunk.
             */
            explicit StandardChunk(size_t elemCount);

            /**
             * @brief Default destructor.
             */
            ~StandardChunk() = default;

            StandardChunk(const StandardChunk &other) = delete;
            StandardChunk(StandardChunk &&other) = delete;
            StandardChunk &operator=(const StandardChunk &other) = delete;
            StandardChunk &operator=(StandardChunk &&other) = delete;

            /**
             * @brief Retrieves a reference to the element at the specified index.
             * 
             * @param idx The index of the element to retrieve.
             * @return T& A reference to the element at the specified index.
             */
            T& getElem(size_t idx) override { return _elems[idx]; }
            
            /**
             * @brief Retrieves a constant reference to the element at the specified index.
             * 
             * @param idx The index of the element to retrieve.
             * @return const T& A constant reference to the element at the specified index.
             */
            const T& getElem(size_t idx) const override { return _elems[idx]; }

            /**
             * @brief Retrieves a reference to the vector of elements.
             * 
             * @return std::vector<T>& A reference to the vector of elements.
             */
            std::vector<T>& getElems() override { return _elems; }

            /**
             * @brief Retrieves a constant reference to the vector of elements.
             * 
             * @return const std::vector<T>& A constant reference to the vector of elements.
             */
            const std::vector<T>& getElems() const override { return _elems; }

        protected:
            std::vector<T> _elems; ///< The vector storing the elements.            std::vector<T> _elems;
        };
    }

