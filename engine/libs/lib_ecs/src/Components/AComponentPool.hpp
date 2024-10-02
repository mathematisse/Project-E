/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base AComponentPool
*/

#pragma once

#include "Chunks/IChunkPool.hpp"
#include "Components/IComponentPool.hpp"
#include "Components/ComponentRefs.hpp"
#include "Chunks/StandardChunkPool.hpp"
#include <tuple>
#include <vector>

namespace ECS::Components {

/**
 * @brief Variadic component pool class template for managing pools of multiple component types.
 * 
 * @tparam Ts Types of the components managed by the pool.
 */
template <typename... Ts>
class AComponentPool : virtual public IComponentPool {
public:
    /**
     * @brief Construct a new AComponentPool object.
     */
    explicit AComponentPool() : IComponentPool(), _pools{Chunks::StandardChunkPool<Ts>()...} {}

    ~AComponentPool() override = default;

    AComponentPool(const AComponentPool&) = default;
    AComponentPool& operator=(const AComponentPool&) = default;
    AComponentPool(AComponentPool&&) = default;
    AComponentPool& operator=(AComponentPool&&) = default;

    /**
     * @brief Retrieve a reference to a component at the specified chunk position.
     * 
     * @param cPos Position of the chunk.
     * @return IComponentRef* Reference to the component.
     */
    IComponentRef* getComponentRef(Chunks::ChunkPos cPos) override {
        return getComponentRefImpl(cPos, std::index_sequence_for<Ts...>{});
    }

    /**
     * @brief Retrieve a dummy reference to a component at the specified chunk position.
     * 
     * @param cPos Position of the chunk.
     * @return const IComponentRef* Dummy reference to the component.
     */
    [[nodiscard]] const IComponentRef* getDummyComponentRef(Chunks::ChunkPos cPos) const override {
        return getDummyComponentRefImpl(cPos, std::index_sequence_for<Ts...>{});
    }

    /**
     * @brief Get the number of elements in the pool.
     * 
     * @return chunk_pos_t Number of elements.
     */
    [[nodiscard]] Chunks::chunk_pos_t elemCount() const override {
        return std::get<0>(_pools).elemCount(); // Assume all pools have the same element count.
    }

    /**
     * @brief Get the number of chunks in the pool.
     * 
     * @return chunk_pos_t Number of chunks.
     */
    [[nodiscard]] Chunks::chunk_pos_t chunkCount() const override {
        return std::get<0>(_pools).chunkCount(); // Assume all pools have the same chunk count.
    }

    /**
     * @brief Add a chunk to the pool with the specified number of elements.
     * 
     * @param elemCount Number of elements in the chunk.
     */
    void addChunk(size_t elemCount) override {
        addChunkImpl(elemCount, std::index_sequence_for<Ts...>{});
    }

    /**
     * @brief Get the chunk pools managed by this component pool.
     * 
     * @return std::vector<Chunks::IChunkPool<Ts>*> Vector of chunk pools.
     */
    std::vector<Chunks::IChunkPool<Ts> *...> getChunkPools() {
        return {&std::get<Chunks::StandardChunkPool<Ts>>(_pools)...};
    }


    std::tuple<std::vector<Ts>&...> getRawStdVectors(size_t index) {
        // Access the chunk pools using index-based access, not type-based.
        return getRawStdVectorsImpl(index, std::index_sequence_for<Ts...>{});
    }

    [[nodiscard]] const std::string &getComponentName() const override {
        return componentName;
    }

    static const std::string componentName; ///< The name of the component pool.

    using Types = std::tuple<Ts&...>; ///< The types of the components managed by the pool.
    using VTypes = std::tuple<std::vector<Ts>&...>; ///< The vector types of the components managed by the pool.

protected:
    std::tuple<Chunks::StandardChunkPool<Ts>...> _pools; ///< The chunk pools for each component type.

private:
    // Helper function to retrieve component reference using index sequence
    template <std::size_t... Indices>
    IComponentRef* getComponentRefImpl(Chunks::ChunkPos cPos, std::index_sequence<Indices...> /*unused*/) {
        return new ComponentRef<Ts...>(std::get<Indices>(_pools).getElem(cPos)...);
    }

    // Helper function to retrieve dummy component reference using index sequence
    template <std::size_t... Indices>
    const IComponentRef* getDummyComponentRefImpl(Chunks::ChunkPos cPos, std::index_sequence<Indices...> /*unused*/) const {
        return new ComponentRef<Ts...>(*(std::get<Indices>(_pools).getElem(cPos))...);
    }

    // Helper function to add chunk for each component type using index sequence
    template <std::size_t... Indices>
    void addChunkImpl(size_t elemCount, std::index_sequence<Indices...>  /*unused*/) {
        (std::get<Indices>(_pools).addChunk(elemCount), ...);
    }

    // Helper function to access the raw vectors by index
    template <std::size_t... Is>
    std::tuple<std::vector<Ts>&...> getRawStdVectorsImpl(size_t index, std::index_sequence<Is...> /*unused*/) {
        return std::tie(
            (*static_cast<Chunks::StandardChunk<Ts> *>(std::get<Is>(_pools).getChunk(index))->getElems())...
        );
    }
};

} // namespace ECS::Components
