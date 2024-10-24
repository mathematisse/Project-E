/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base Components
*/

#pragma once

#include <tuple>
#include "lib_ecs/Components/ComponentVal.hpp"

namespace ECS::C {

template<typename... Ts>
class ComponentRef {
public:
    /**
     * @brief Construct a new ComponentRef object with component pointers.
     *
     * @param components Pointers to the component values.
     */
    ComponentRef(Ts *...components):
        _components(std::make_tuple(components...))
    {
    }

    /**
     * @brief Get the component value pointer at the specified index.
     *
     * @tparam Index Index of the component.
     * @return Pointer to the component value.
     */
    template<std::size_t Index>
    decltype(auto) get()
    {
        return std::get<Index>(_components);
    }

    /**
     * @brief Get the component value pointer at the specified index (const version).
     *
     * @tparam Index Index of the component.
     * @return Pointer to the component value.
     */
    template<std::size_t Index>
    decltype(auto) get() const
    {
        return std::get<Index>(_components);
    }

    /**
     * @brief Set the component value at the specified index.
     *
     * @tparam Index Index of the component.
     * @param value New value for the component.
     */
    template<std::size_t Index>
    void set(const std::tuple_element_t<Index, std::tuple<Ts...>> &value)
    {
        *std::get<Index>(_components) = value;
    }

    void set(const ComponentRef<Ts...> &ref)
    {
        set(ref._components, std::index_sequence_for<Ts...> {});
    }

    void set(const ComponentVal<Ts...> &val)
    {
        set(val._values, std::index_sequence_for<Ts...> {});
    }

    template<std::size_t... Is>
    void set(const std::tuple<Ts *...> &components, std::index_sequence<Is...> /*unused*/)
    {
        (set<Is>(*std::get<Is>(components)), ...);
    }

    template<std::size_t... Is>
    void set(const std::tuple<Ts...> &components, std::index_sequence<Is...> /*unused*/)
    {
        (set<Is>(std::get<Is>(components)), ...);
    }

protected:
    std::tuple<Ts *...> _components; ///< Tuple of pointers to component values.

private:
    /**
     * @brief Creates a tuple of pointers to the provided components.
     *
     * @param components Component values.
     * @return Tuple of pointers to the values.
     */
    static auto createPointerTuple(Ts... components) { return std::make_tuple(&components...); }
};

} // namespace ECS::C
