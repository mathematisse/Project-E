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

class IComponentRef {
public:
    virtual ~IComponentRef() = default;
};

template<typename... Ts>
class ComponentRef : public IComponentRef {
public:
    /**
     * @brief Construct a new ComponentRef object with component pointers.
     *
     * @param components Pointers to the component values.
     */
    explicit ComponentRef(Ts *...components):
        _components(std::make_tuple(components...))
    {
    }

    ~ComponentRef() override {
        // do nothing
    };

    // move constructor
    ComponentRef(ComponentRef &&other) noexcept:
        _components(std::move(other._components))
    {
    }

    // copy constructor
    ComponentRef(const ComponentRef &other):
        _components(other._components)
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

    operator ComponentVal<Ts...>() const { return ComponentVal<Ts...>(_components); }

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

    // Automatic casting to the first element
    operator std::tuple_element_t<0, std::tuple<Ts...>>() const { return std::get<0>(_components); }

    // auto casting to tuple
    operator std::tuple<Ts...>() const { return _components; }

protected:
    std::tuple<Ts *...> _components; ///< Tuple of pointers to component values.
};

} // namespace ECS::C
