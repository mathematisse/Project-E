/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base Components
*/

#pragma once

#include <tuple>

namespace ECS::C {

class IComponentVal {
public:
    virtual ~IComponentVal() = default;
};

template<typename... Ts>
class ComponentVal : public IComponentVal {
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

    static constexpr const ComponentVal Zero() { return ComponentVal(std::tuple<Ts...> {}); }

    ComponentVal():
        _values(std::tuple<Ts...> {})
    {
    }

    explicit ComponentVal(Ts *...components):
        _values(*components...)
    {
    }

    explicit ComponentVal(const Ts *...components):
        _values(*components...)
    {
    }

    // tuple versions
    explicit ComponentVal(std::tuple<Ts...> components):
        _values(components)
    {
    }

    // Helper to dereference tuple of pointers
    template<std::size_t... Is>
    ComponentVal(std::tuple<Ts *...> components, std::index_sequence<Is...>):
        _values(std::make_tuple((*std::get<Is>(components))...))
    {
    }

    // Constructor using the helper
    explicit ComponentVal(std::tuple<Ts *...> components):
        ComponentVal(components, std::index_sequence_for<Ts...> {})
    {
    }

    // ComponentVal(std::tuple<Ts *...> components):
    //     _values(components)
    // {
    // }

    ~ComponentVal() override = default;

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

    void set(const std::tuple<Ts...> &values) { _values = values; }

    void set(const ComponentVal<Ts...> &values) { _values = values._values; }

    template<std::size_t Index>
    void set(const std::tuple_element_t<Index, std::tuple<Ts...>> &value)
    {
        std::get<Index>(_values) = value;
    }

    std::tuple<Ts...> _values; ///< Tuple of component values for dummy version.

    // Automatic casting to the first element
    operator std::tuple_element_t<0, std::tuple<Ts...>>() const { return std::get<0>(_values); }

    // auto casting to tuple
    operator std::tuple<Ts...>() const { return _values; }
};

} // namespace ECS::C
