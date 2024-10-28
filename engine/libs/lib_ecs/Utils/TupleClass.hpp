/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include <tuple>
#include <vector>

// ArchetypeVal class template
template<typename Tuple>
class TupleClass;

template<typename... TElems>
class TupleClass<std::tuple<TElems...>> {
public:
    TupleClass() = default;

    explicit TupleClass(std::tuple<TElems...> &elems):
        _elems(std::move(elems))
    {
    }

    explicit TupleClass(const std::tuple<TElems...> &elems):
        _elems(elems)
    {
    }

    template<std::size_t Index>
    inline decltype(auto) get()
    {
        return std::get<Index>(_elems);
    }

    template<std::size_t Index>
    [[nodiscard]] inline decltype(auto) get() const
    {
        return std::get<Index>(_elems);
    }

    std::tuple<TElems...> &getTuple() { return _elems; }

    template<std::size_t Index>
    inline void set(const std::tuple_element_t<Index, std::tuple<TElems...>> &elem)
    {
        std::get<Index>(_elems).set(elem);
    }

    inline void set(const std::tuple<TElems...> &elems) { _elems = elems; }

    template<typename TInterface>
    std::array<TInterface *, sizeof...(TElems)> getInterfacePtr()
    {
        return getInterfacePtrHelper<TInterface>(std::make_index_sequence<sizeof...(TElems)> {});
    }

private:
    template<typename TInterface, std::size_t... Indices>
    std::array<TInterface *, sizeof...(TElems)>
    getInterfacePtrHelper(std::index_sequence<Indices...> /*unused*/)
    {
        return {(static_cast<TInterface *>(&std::get<Indices>(_elems)))...};
    }

protected:
    std::tuple<TElems...> _elems;
};
