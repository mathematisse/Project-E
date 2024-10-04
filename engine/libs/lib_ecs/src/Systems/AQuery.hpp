/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "Components/IComponentPool.hpp"
#include "Entities/IEntityPool.hpp"
#include "Systems/IQuery.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <vector>


namespace ECS::S {

// Helper metafunction to check if all types are derived from Base
template<typename... Ts> struct all_derived_from_base;

template<typename T> struct all_derived_from_base<T> : std::is_base_of<C::IComponentPool, T>
{
};

template<typename T, typename... Ts>
struct all_derived_from_base<T, Ts...>
  : std::conjunction<std::is_base_of<C::IComponentPool, T>, all_derived_from_base<Ts...>>
{
};


template<typename... Ts> class Query : public IQuery
{

  // Enable this class only if all types in Ts are derived from Base
  static_assert(all_derived_from_base<Ts...>::value, "All types must be derived from IComponentPool");

public:
  bool tryAddEntityPool(E::IEntityPool *entityPool) override
  {
    std::vector<C::IComponentPool *> componentPools = entityPool->getComponentPools();

    std::array<const std::string, sizeof...(Ts)> componentNames = { Ts::componentName... };
    std::array<C::IComponentPool *, sizeof...(Ts)> newComponentPoolsArray = { nullptr };

    size_t i = 0;

    for (auto &componentPool : componentPools) {
      for (auto &componentName : componentNames) {
        if (componentPool->getComponentName() == componentName) {
          if (i == sizeof...(Ts)) { return false; }
          newComponentPoolsArray[i] = componentPool;
          i++;
        }
      }
    }
    if (i == sizeof...(Ts)) {
      std::cout << "Added entity pool " << entityPool->getEntityName() << " to system\n";
      _componentPoolsArrays.push_back(newComponentPoolsArray);
      return true;
    }
    return false;
  }

  using TLambda = std::function<void(typename Ts::Types &...)>;

  void iterate(TLambda &f)
  {
    // print the name of the class
    std::cout << "\nRunning system " << typeid(*this).name() << "\n";
    for (auto &componentPools : _componentPoolsArrays) {
      size_t chunkCount = componentPools[0]->chunkCount();
      for (size_t i = 0; i < chunkCount; i++) { _run(f, componentPools, i); }
    }
  }

protected:
  void _run(TLambda &f, std::array<C::IComponentPool *, sizeof...(Ts)> &componentPools, size_t index)
  {
    auto componentPoolsTuple = std::apply(
      [index](auto &...pools) { return std::make_tuple(dynamic_cast<Ts *>(pools)->getRawStdVectors(index)...); },
      componentPools);

    std::apply([this](TLambda &f, auto &...componentPools) { _operate(f, componentPools...); }, componentPoolsTuple);
  }

  void _operate(TLambda &f, typename Ts::VTypes &...componentVectors)
  {
    size_t vectorSize = std::get<0>(std::get<0>(std::tie(componentVectors...))).size();

    auto vectorTuple = std::tie(componentVectors...);

    using OuterIndices = std::make_index_sequence<std::tuple_size<decltype(vectorTuple)>::value>;

    for (size_t i = 0; i < vectorSize; ++i) {
      auto refTuple = _getReferencesAtIndex(i, vectorTuple, OuterIndices{});
      _callInnerOperate(f, refTuple, std::make_index_sequence<std::tuple_size<decltype(refTuple)>::value>{});
    }
    std::cout << "\n";
  }

  template<std::size_t... Is, typename... Vectors>
  constexpr inline auto
    _getInnerReferencesAtIndex(size_t i, std::tuple<Vectors...> &vectorTuple, std::index_sequence<Is...> /*unused*/)
  {
    return std::make_tuple(std::ref(std::get<Is>(vectorTuple)[i])...);
  }

  template<std::size_t... OuterIs, typename... InnerTuples>
  constexpr inline auto
    _getReferencesAtIndex(size_t i, std::tuple<InnerTuples...> &outerTuple, std::index_sequence<OuterIs...> /*unused*/)
  {
    return std::make_tuple(_getInnerReferencesAtIndex(i,
      std::get<OuterIs>(outerTuple),
      std::make_index_sequence<std::tuple_size<std::decay_t<decltype(std::get<OuterIs>(outerTuple))>>::value>{})...);
  }

  template<typename Tuple, std::size_t... Is>
  constexpr inline void _callInnerOperate(TLambda &f, Tuple &refTuple, std::index_sequence<Is...> /*unused*/)
  {
    f(std::get<Is>(refTuple)...);
  }

  std::vector<std::array<C::IComponentPool *, sizeof...(Ts)>> _componentPoolsArrays;
};

}// namespace ECS::S
