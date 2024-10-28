/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ComponentPool
*/

#pragma once

#include "lib_ecs/Chunks/StandardChunkPool.hpp"
#include "lib_ecs/Components/ComponentRef.hpp"
#include "lib_ecs/Components/IComponentPool.hpp"
#include <span>
#include <tuple>
#include <vector>

namespace ECS::C {

/**
 * @brief Variadic component pool class template for managing pools of multiple component types.
 *
 * @tparam Ts Types of the components managed by the pool.
 */
template<const char *Name, typename... Ts>
class ComponentPool : public IComponentPool {
public:
    /**
     * @brief Construct a new ComponentPool object.
     */
    explicit ComponentPool():
        IComponentPool(),
        _pools {Chunks::StandardChunkPool<Ts>()...}
    {
    }

    ~ComponentPool() override = default;

    ComponentPool(const ComponentPool &) = default;
    ComponentPool &operator=(const ComponentPool &) = default;
    ComponentPool(ComponentPool &&) = default;
    ComponentPool &operator=(ComponentPool &&) = default;

    using Types = std::tuple<Ts &...>; ///< The types of the components managed by the pool.
    using VTypes = std::tuple<std::vector<Ts> &...>; ///< The vector types of the components managed
                                                     ///< by the pool.
    using ATypes = std::tuple<std::vector<Ts> *...>; ///< The vector pointer types of the components
                                                     ///< managed by the pool.

    /**
     * @brief Retrieve a reference to a component at the specified chunk position.
     *
     * @param cPos Position of the chunk.
     * @return ComponentRef<Ts...> ref of the component.
     */
    ComponentRef<Ts...> getComponentRef(Chunks::chunkPos_t cPos)
    {
        return getComponentRefImpl(cPos, std::index_sequence_for<Ts...> {});
    }

    /**
     * @brief Retrieve a dummy reference to a component at the specified chunk position.
     *
     * @param cPos Position of the chunk.
     * @return const ComponentRef<Ts...> Dummy ref to the component.
     */
    [[nodiscard]] ComponentVal<Ts...> getComponentVal(Chunks::chunkPos_t cPos) const
    {
        return getComponentValImpl(cPos, std::index_sequence_for<Ts...> {});
    }

    void resetComponentAtIndex(const Chunks::chunkPos_t &index) override
    {
        setComponentAtIndex(index, std::tuple<Ts...> {});
    }

    void setComponentAtIndex(const Chunks::chunkPos_t &index, const std::tuple<Ts...> &component)
    {
        setComponentAtIndexImpl(index, component, std::index_sequence_for<Ts...> {});
    }

    void resetComponentAtIndexes(const Chunks::cPosArr_t &indexes) override
    {
        for (const auto &index : indexes) {
            setComponentAtIndex(index, std::tuple<Ts...> {});
        }
    }

    /**
     * @brief Set components at the specified indexes.
     * @param indexes The indexes at which to set the components.
     * @param component The component to set.
     */
    void setComponentAtIndexes(const Chunks::cPosArr_t &indexes, const std::tuple<Ts...> &component)
    {
        setComponentAtIndexesImpl(indexes, component, std::index_sequence_for<Ts...> {});
    }

    // same but with span
    void setComponentAtIndexes(
        const std::span<Chunks::chunkPos_t> &indexes, const std::tuple<Ts...> &component
    )
    {
        setComponentAtIndexesImpl(indexes, component, std::index_sequence_for<Ts...> {});
    }

    // same with range
    void setComponentAtIndexes(
        std::ranges::input_range auto &&indexes, const std::tuple<Ts...> &component
    )
    {
        for (const auto &index : indexes) {
            setComponentAtIndex(index, component);
        }
    }

    void setComponentsAtIndexes(
        const Chunks::cPosArr_t &indexes, const std::vector<std::tuple<Ts...>> &components
    )
    {
        for (size_t i = 0; i < indexes.size(); i++) {
            setComponentAtIndex(indexes[i], components[i]);
        }
    }

    // same but with span
    void setComponentsAtIndexes(
        const std::span<Chunks::chunkPos_t> &indexes, const std::span<std::tuple<Ts...>> &components
    )
    {
        for (size_t i = 0; i < indexes.size(); i++) {
            setComponentAtIndex(indexes[i], components[i]);
        }
    }

    // same but with range
    void setComponentsAtIndexes(
        std::ranges::input_range auto &&indexes, const std::vector<std::tuple<Ts...>> &components
    )
    {
        for (size_t i = 0; i < indexes.size(); i++) {
            setComponentAtIndex(indexes[i], components[i]);
        }
    }

    void getComponentsAtIndexes(
        const Chunks::cPosArr_t &indexes, std::vector<std::tuple<Ts...>> &components
    )
    {
        getComponentsAtIndexesImpl(indexes, components, std::index_sequence_for<Ts...> {});
    }

    // same but with span
    void getComponentsAtIndexes(
        std::span<Chunks::chunkPos_t> indexes, std::vector<std::tuple<Ts...>> &components
    )
    {
        getComponentsAtIndexesImpl(indexes, components, std::index_sequence_for<Ts...> {});
    }

    // same but with range
    void getComponentsAtIndexes(
        std::ranges::input_range auto &&indexes, std::vector<std::tuple<Ts...>> &components
    )
    {
        getComponentsAtIndexesImpl(indexes, components, std::index_sequence_for<Ts...> {});
    }

    /**
     * @brief Get the number of elements in the pool.
     *
     * @return chunk_pos_t Number of elements.
     */
    [[nodiscard]] Chunks::chunk_idx_t elemCount() const override
    {
        return std::get<0>(_pools).elemCount(); // Assume all pools have the same element count.
    }

    /**
     * @brief Get the number of chunks in the pool.
     *
     * @return chunk_pos_t Number of chunks.
     */
    [[nodiscard]] Chunks::chunk_idx_t chunkCount() const override
    {
        return std::get<0>(_pools).chunkCount(); // Assume all pools have the same chunk count.
    }

    /**
     * @brief Add a chunk to the pool with the specified number of elements.
     *
     * @param elemCount Number of elements in the chunk.
     */
    void addChunk(size_t elemCount) override
    {
        addChunkImpl(elemCount, std::index_sequence_for<Ts...> {});
    }

    /**
     * @brief Get the chunk pools managed by this component pool.
     *
     * @return std::tuple<Chunks::StandardChunkPool<Ts>...> Tuple of chunk pools.
     */
    std::tuple<Chunks::StandardChunkPool<Ts>...> &getChunkPools() { return _pools; }

    VTypes getRawStdVectors(size_t index)
    {
        // Access the chunk pools using index-based access, not type-based.
        return getRawStdVectorsImpl(index, std::index_sequence_for<Ts...> {});
    }

    ATypes getRawAdrrStdVectors(size_t index)
    {
        // Access the chunk pools using index-based access, not type-based.
        return getRawStdVectorsImpl(index, std::index_sequence_for<Ts...> {});
    }

    [[nodiscard]] const char *getComponentName() const override { return componentName; }

    static constexpr const char *componentName = Name;

    void deleteEverything()
    {
        std::apply(
            [](auto &...pools) {
                (pools.deleteEverything(), ...);
            },
            _pools
        );
    }

protected:
    std::tuple<Chunks::StandardChunkPool<Ts>...>
        _pools; ///< The chunk pools for each component type.

private:
    template<std::size_t... Indices>
    inline void setComponentAtIndexImpl(
        const Chunks::chunkPos_t &index, const std::tuple<Ts...> &component,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        (std::get<Indices>(_pools).setValueAtIndexes({index}, std::get<Indices>(component)), ...);
    }

    template<std::size_t... Indices>
    inline void setComponentAtIndexesImpl(
        const Chunks::cPosArr_t &indexes, const std::tuple<Ts...> &component,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        (std::get<Indices>(_pools).setValueAtIndexes(indexes, std::get<Indices>(component)), ...);
    }

    // same with span
    template<std::size_t... Indices>
    inline void setComponentAtIndexesImpl(
        std::span<Chunks::chunkPos_t> indexes, const std::tuple<Ts...> &component,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        (std::get<Indices>(_pools).setValueAtIndexes(indexes, std::get<Indices>(component)), ...);
    }

    // same with range
    template<std::size_t... Indices>
    inline void setComponentAtIndexesImpl(
        std::ranges::input_range auto &&indexes, const std::tuple<Ts...> &component,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        for (const auto &index : indexes) {
            setComponentAtIndex(index, component);
        }
    }

    template<std::size_t... Indices>
    inline void getComponentsAtIndexesImpl(
        const Chunks::cPosArr_t &indexes, std::vector<std::tuple<Ts...>> &components,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        ((std::get<Indices>(_pools).template getValuesAtIndexes<Indices, Ts...>(indexes, components)
         ),
         ...);
    }

    // same with span
    template<std::size_t... Indices>
    inline void getComponentsAtIndexesImpl(
        std::span<Chunks::chunkPos_t> indexes, std::vector<std::tuple<Ts...>> &components,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        ((std::get<Indices>(_pools).template getValuesAtIndexes<Indices, Ts...>(indexes, components)
         ),
         ...);
    }
    // same with range
    template<std::size_t... Indices>
    inline void getComponentsAtIndexesImpl(
        std::ranges::input_range auto &&indexes, std::vector<std::tuple<Ts...>> &components,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        getComponentsAtIndexesRangeImpl(
            std::forward<decltype(indexes)>(indexes), components, std::index_sequence_for<Ts...> {}
        );
    }

    // Helper function to retrieve components using range-based index sequence
    template<std::ranges::input_range R, std::size_t... Indices>
    void getComponentsAtIndexesRangeImpl(
        R &&indexes, std::vector<std::tuple<Ts...>> &components,
        std::index_sequence<Indices...> /*unused*/
    )
    {
        ((std::get<Indices>(_pools).template getValuesAtIndexes<Indices, Ts...>(
             std::forward<R>(indexes), components
         )),
         ...);
    }

    // Helper function to retrieve component reference using index sequence
    template<std::size_t... Indices>
    ComponentRef<Ts...>
    getComponentRefImpl(Chunks::chunkPos_t cPos, std::index_sequence<Indices...> /*unused*/)
    {
        return ComponentRef<Ts...>(std::get<Indices>(_pools).getElem(cPos)...);
    }

    // Helper function to retrieve dummy component reference using index sequence
    template<std::size_t... Indices>
    ComponentVal<Ts...>
    getComponentValImpl(Chunks::chunkPos_t cPos, std::index_sequence<Indices...> /*unused*/) const
    {
        return ComponentVal<Ts...>((std::get<Indices>(_pools).getElem(cPos))...);
    }

    // Helper function to add chunk for each component type using index sequence
    template<std::size_t... Indices>
    void addChunkImpl(size_t elemCount, std::index_sequence<Indices...> /*unused*/)
    {
        (std::get<Indices>(_pools).addChunk(elemCount), ...);
    }

    // Helper function to access the raw vectors by index
    template<std::size_t... Is>
    VTypes getRawStdVectorsImpl(size_t index, std::index_sequence<Is...> /*unused*/)
    {
        return std::tie(
            (static_cast<Chunks::StandardChunk<Ts> *>(std::get<Is>(_pools).getChunk(index))
                 ->getElems())...
        );
    }

    // Ensure that getRawStdVectorsImpl returns pointers to vectors.
    template<size_t... Is>
    ATypes *getRawAddrStdVectorsImpl(size_t index, std::index_sequence<Is...> /*unused*/)
    {
        return std::tie((std::get<Is>(_pools).getChunk(index)->getElems())...);
    }
};

} // namespace ECS::C
