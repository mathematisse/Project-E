/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base Components
*/

#pragma once

#include <tuple>

namespace ECS::C {

template<typename... Ts>
class ComponentVal {
public:
    /**
     * @brief Construct a new ComponentVal object with component values.
     *
     * @param components Component values.
     */
    ComponentVal(Ts... components):
        _values(components...)
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
        return std::get<Index>(_values);
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
        return std::get<Index>(_values);
    }

    std::tuple<Ts...> _values; ///< Tuple of component values for dummy version.
};

} // namespace ECS::C
