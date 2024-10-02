/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base Components
*/

#pragma once

#include "Components/IComponentRef.hpp"
#include <tuple>

namespace ECS::Components {

template <typename... Ts>
class ComponentRef : public IComponentRef {
public:
    /**
     * @brief Construct a new ComponentRef object with component pointers.
     * 
     * @param components Pointers to the component values.
     */
    explicit ComponentRef(Ts*... components) 
        : _components(std::make_tuple(components...)) {}

    /**
     * @brief Construct a new ComponentRef object with component values.
     * 
     * @param components Component values.
     */
    explicit ComponentRef(Ts... components) 
        : _components(createPointerTuple(components...)) {}

    /**
     * @brief Get the component value pointer at the specified index.
     * 
     * @tparam Index Index of the component.
     * @return Pointer to the component value.
     */
    template <std::size_t Index>
    decltype(auto) get() {
        return std::get<Index>(_components);
    }

    /**
     * @brief Get the component value pointer at the specified index (const version).
     * 
     * @tparam Index Index of the component.
     * @return Pointer to the component value.
     */
    template <std::size_t Index>
    decltype(auto) get() const {
        return std::get<Index>(_components);
    }

    /**
     * @brief Set the component value at the specified index.
     * 
     * @tparam Index Index of the component.
     * @param value New value for the component.
     */
    template <std::size_t Index>
    void set(const std::tuple_element_t<Index, std::tuple<Ts...>>& value) {
        *std::get<Index>(_components) = value;
    }

protected:
    std::tuple<Ts*...> _components; ///< Tuple of pointers to component values.

private:
    /**
     * @brief Creates a tuple of pointers to the provided components.
     * 
     * @param components Component values.
     * @return Tuple of pointers to the values.
     */
    static auto createPointerTuple(Ts... components) {
        return std::make_tuple(&components...);
    }
};

} // namespace ECS::Components
