/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "Components/IComponentPool.hpp"
#include "Entities/IEntityPool.hpp"
#include "Systems/ASystem.hpp"
#include <iostream>
#include <string>
#include <vector>


namespace ECS::Systems {

template<typename Tuple1, typename Tuple2> class ADualSystem;

template<typename... T1s, typename... T2s> class ADualSystem<std::tuple<T1s...>, std::tuple<T2s...>> : public ASystem
{
  static_assert(all_derived_from_base<T1s...>::value, "All types in T1s must be derived from IComponentPool");
  static_assert(all_derived_from_base<T2s...>::value, "All types in T2s must be derived from IComponentPool");

public:
  explicit ADualSystem(bool isParallel) : ASystem(isParallel) {}
  ~ADualSystem() override = default;

  ADualSystem(const ADualSystem &other) = default;
  ADualSystem(ADualSystem &&other) = default;
  ADualSystem &operator=(const ADualSystem &other) = default;
  ADualSystem &operator=(ADualSystem &&other) = default;

  bool tryAddEntityPool(Entities::IEntityPool *entityPool) override
  {
    std::vector<Components::IComponentPool *> componentPools = entityPool->getComponentPools();

    std::array<const std::string, sizeof...(T1s)> _componentNames1 = { T1s::componentName... };
    std::array<const std::string, sizeof...(T2s)> _componentNames2 = { T2s::componentName... };

    std::array<Components::IComponentPool *, sizeof...(T1s)> newComponentPoolsArray1 = { nullptr };
    std::array<Components::IComponentPool *, sizeof...(T2s)> newComponentPoolsArray2 = { nullptr };

    size_t i = 0;
    size_t j = 0;

    for (auto &componentPool : componentPools) {
      for (auto &componentName : _componentNames1) {
        if (componentPool->getComponentName() == componentName) {
          if (i < sizeof...(T1s)) { newComponentPoolsArray1[i] = componentPool; }
          i++;
        }
      }
      for (auto &componentName : _componentNames2) {
        if (componentPool->getComponentName() == componentName) {
          if (j < sizeof...(T2s)) { newComponentPoolsArray2[j] = componentPool; }
          j++;
        }
      }
    }
    if (i == sizeof...(T1s)) {
      std::cout << "Added entity pool " << entityPool->getEntityName() << " to system (1)\n";
      _componentPoolsArrays1.push_back(newComponentPoolsArray1);
    }
    if (j == sizeof...(T2s)) {
      std::cout << "Added entity pool " << entityPool->getEntityName() << " to system (2)\n";
      _componentPoolsArrays2.push_back(newComponentPoolsArray2);
    }
    return (i == sizeof...(T1s) || j == sizeof...(T2s));
  }

  void run() override
  {
    // print the name of the class
    std::cout << "\nRunning system " << typeid(*this).name() << "\n";
    for (size_t i = 0; i < _componentPoolsArrays1.size(); i++) {
      for (size_t j = 0; j < _componentPoolsArrays2.size(); j++) {
        size_t chunkCount1 = _componentPoolsArrays1[i][0]->chunkCount();
        size_t chunkCount2 = _componentPoolsArrays2[j][0]->chunkCount();
        for (size_t k = 0; k < chunkCount1; k++) {
          for (size_t l = 0; l < chunkCount2; l++) { _run(_componentPoolsArrays1[i], _componentPoolsArrays2[j], k, l); }
          std::cout << "\nnew chunk iteration\n";
        }
      }
    }
  }

protected:
  void _run(std::array<Components::IComponentPool *, sizeof...(T1s)> &componentPools1,
    std::array<Components::IComponentPool *, sizeof...(T2s)> &componentPools2,
    size_t index1,
    size_t index2)
  {
    auto componentPoolsTuple1 = std::apply(
      [index1](auto &...pools) { return std::make_tuple(dynamic_cast<T1s *>(pools)->getRawStdVectors(index1)...); },
      componentPools1);
    auto componentPoolsTuple2 = std::apply(
      [index2](auto &...pools) { return std::make_tuple(dynamic_cast<T2s *>(pools)->getRawStdVectors(index2)...); },
      componentPools2);

    std::apply(
      [this, &componentPoolsTuple1, &componentPoolsTuple2](auto &...componentPools1) {
        std::apply(
          [this, &componentPools1...](auto &...componentPools2) { _operate(componentPools1..., componentPools2...); },
          componentPoolsTuple2);
      },
      componentPoolsTuple1);
  }

  inline void _operate(typename T1s::VTypes &...componentVectors1, typename T2s::VTypes &...componentVectors2)
  {
    size_t vectorSize1 = std::get<0>(std::get<0>(std::tie(componentVectors1...))).size();
    size_t vectorSize2 = std::get<0>(std::get<0>(std::tie(componentVectors2...))).size();

    auto vectorTuple1 = std::tie(componentVectors1...);
    auto vectorTuple2 = std::tie(componentVectors2...);

    using OuterIndices1 = std::make_index_sequence<std::tuple_size<decltype(vectorTuple1)>::value>;
    using OuterIndices2 = std::make_index_sequence<std::tuple_size<decltype(vectorTuple2)>::value>;

    for (size_t i = 0; i < vectorSize1; ++i) {
      for (size_t j = 0; j < vectorSize2; ++j) {
        auto refTuple1 = _getReferencesAtIndex(i, vectorTuple1, OuterIndices1{});
        auto refTuple2 = _getReferencesAtIndex(j, vectorTuple2, OuterIndices2{});
        _callInnerOperate(
          refTuple1, refTuple2, std::make_index_sequence<std::tuple_size<decltype(refTuple1)>::value>{});
      }
      std::cout << " ";
    }
    std::cout << "\n";
  }

  virtual inline void _innerOperate(typename T1s::Types &...componentPools1,
    typename T2s::Types &...componentPools2) = 0;

  template<typename Tuple1, typename Tuple2, std::size_t... Is>
  constexpr inline void _callInnerOperate(Tuple1 &refTuple1, Tuple2 &refTuple2, std::index_sequence<Is...> /*unused*/)
  {
    _innerOperate(std::get<Is>(refTuple1)..., std::get<Is>(refTuple2)...);
  }

  std::vector<std::array<Components::IComponentPool *, sizeof...(T1s)>> _componentPoolsArrays1;
  std::vector<std::array<Components::IComponentPool *, sizeof...(T2s)>> _componentPoolsArrays2;
};
}// namespace ECS::Systems
