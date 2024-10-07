
#pragma once

#include "ComponentStatus.hpp"
#include <algorithm> // for std::find_if
#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <tuple>
#include <vector>

template<typename... Components>
requires are_types_unique_v<Components...>
class World {
public:
    class Exist;

    template<typename T>
    static constexpr bool are_from_components_v = (std::is_same_v<T, Components> || ...);

    template<typename T>
    using container_t = std::vector<T>;

    using tables_t = std::tuple<container_t<Components>...>;
    using status_table_t = container_t<ComponentStatus<Exist, Components...>>;

private:
    static constexpr size_t defaultTableCapacity = 8;

    status_table_t status;
    tables_t tables;
    size_t tables_capacity = defaultTableCapacity;
    size_t number_of_entities = 0;

private:
    void increase_capacity(size_t new_capacity)
    {
        std::cout << "Increasing capacity " << tables_capacity << " to " << new_capacity << std::endl;
        tables_capacity = new_capacity;
        (std::get<container_t<Components>>(tables).resize(tables_capacity), ...);
        status.resize(tables_capacity);
    }

public:
    World() { increase_capacity(defaultTableCapacity); }

    // template<typename C>
    //     requires is_component_v<C>
    // inline auto get(size_t idx) -> std::optional<C>
    // {
    //     if (status[idx].template isActive<C>()) {
    //         return std::optional<C>(std::get<container_t<C>>(tables)[idx]);
    //     }
    //     return std::nullopt;
    // }
    template<typename... Cs>
    requires are_types_unique_v<Cs...> &&(World::are_from_components_v<Cs> &&...) inline auto get(size_t idx)
        -> std::optional<std::tuple<Cs &...>>
    {
        if ((status[idx].template isActive<Cs>() && ...)) {
            return std::make_optional(std::tie(std::get<container_t<Cs>>(tables)[idx]...));
        }
        return std::nullopt;
    }

    // template<typename... Cs>
    //     requires are_types_unique_v<Cs...> && (World::are_from_components_v<Cs> && ...)
    // inline auto set(size_t idx, Cs &&...components) -> void
    // {
    //     ((std::get<container_t<Cs>>(tables)[idx] = std::forward<Cs>(components)), ...);
    // }

    template<typename... Cs>
    requires are_types_unique_v<Cs...> &&(World::are_from_components_v<Cs> &&...) inline auto has(size_t idx)
        -> bool
    {
        return (status[idx].template isActive<Cs>() && ...);
    }

    template<typename C>
    requires are_from_components_v<C>
    inline auto add(size_t idx, C &&component) -> void
    {
        std::get<container_t<C>>(tables)[idx] = std::forward<C>(component);
        status[idx].template activate<C>();
    }

    template<typename C>
    requires are_from_components_v<C>
    inline auto remove(size_t idx) -> void { status[idx].template deactivate<C>(); }

private:
    // returns the first NonExisting entity index or the tables_capacity if there are no more entities
    inline auto get_next_entity_id() -> size_t
    {
        size_t i = 0;
        if (number_of_entities == tables_capacity) {
            return tables_capacity;
        }
        for (size_t i = 0; i < tables_capacity && i <= number_of_entities; i++) {
            if (!status[i].template isActive<Exist>()) {
                return i;
            }
        }
        return i;
    }

public:
    inline auto new_entity() -> size_t
    {
        size_t idx = get_next_entity_id();
        number_of_entities++;
        if (idx == tables_capacity) {
            increase_capacity(tables_capacity * 2);
            idx = number_of_entities;
        }
        status[idx].template activate<Exist>();
        (status[idx].template deactivate<Components>(), ...);
        return idx;
    }

    inline void delete_entity(size_t idx)
    {
        number_of_entities--;
        status[idx].template deactivate<Exist>();
    }

    [[nodiscard]] inline auto size() const -> size_t { return number_of_entities; }

    [[nodiscard]] inline auto capacity() const -> size_t { return tables_capacity; }

    inline auto clear() -> void
    {
        number_of_entities = 0;
        (std::get<container_t<Components>>(tables).clear(), ...);
        status.clear();
    }

    // begin and end iterators for the world return indices of alive entities
    template<typename... Cs>
    // requires are_types_unique_v<Cs...> && (is_component_v<Cs> && ...)
    struct iterator {
    public:
        using value_type = size_t;
        using reference = size_t &;
        using pointer = size_t *;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

    private:
        const World &world;
        size_t idx;

    public:
        iterator(const World &world, size_t idx):
            world(world),
            idx(idx)
        {
        }

        // increment until we find an alive entity or reach the end
        inline auto operator++() -> iterator &
        {
            idx++;
            while (idx < world.number_of_entities && ((!world.status[idx].template isActive<Cs>()) && ...) &&
                   !world.status[idx].template isActive<Exist>()) {
                idx++;
            }
            return *this;
        }

        // inline auto operator++(int) -> iterator
        // {
        //     iterator it = *this;
        //     ++(*this);
        //     return it;
        // }

        inline auto operator*() -> size_t { return idx; }

        inline auto operator==(const iterator &other) const -> bool { return idx == other.idx; }

        inline auto operator!=(const iterator &other) const -> bool { return idx != other.idx; }
    };

    inline auto begin() const -> iterator<Exist>
    {
        size_t idx = 0;
        while (idx < number_of_entities && !status[idx].template isActive<Exist>()) {
            idx++;
        }
        return iterator<Exist>(*this, idx);
    }

    inline auto end() const -> iterator<Exist> { return iterator<Exist>(*this, number_of_entities); }

    template<typename... FilterComponents>
    requires are_types_unique_v<FilterComponents...>
    class View {
    public:
        using iterator = typename World::template iterator<FilterComponents...>;

    private:
        const World &world;

    public:
        View(const World &world):
            world(world)
        {
        }
        [[nodiscard]] inline auto begin() const -> iterator { return iterator(world, 0); }

        [[nodiscard]] inline auto end() const -> iterator
        {
            return iterator(world, world.number_of_entities);
        }
    };

    template<typename... Cs>
    requires are_types_unique_v<Cs...> &&(World::are_from_components_v<Cs> &&...)
        [[nodiscard]] inline auto view() const -> View<Cs...>
    {
        return View<Cs...>(*this);
    }
    // template<typename... Cs>
    //     requires are_types_unique_v<Cs...> && (is_component_v<Cs> && ...)
    // inline auto begin() const -> iterator<Exist, Cs...>
    // {
    //     size_t idx = 0;
    //     while (idx < number_of_entities &&
    //            (!status[idx].template isActive<Exist>() || (!status[idx].template isActive<Cs>() && ...)))
    //            {
    //         idx++;
    //     }
    //     return iterator<Exist, Cs...>(*this, idx);
    // }

    // template<typename... Cs>
    //     requires are_types_unique_v<Cs...> && (is_component_v<Cs> && ...)
    // inline auto end() const -> iterator<Exist, Cs...>
    // {
    //     return iterator<Exist, Cs...>(*this, number_of_entities);
    // }
};
