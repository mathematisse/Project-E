/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "Components/IComponentPool.hpp"
#include "Entities/IEntityPool.hpp"
#include "EntityManager.hpp"
#include "Systems/IQuery.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template<typename First, typename... Rest> constexpr inline First getFirstArg(First first, Rest... /*unused*/)
{
  return first;
}


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
      std::cout << "Added entity pool " << entityPool->getEntityName() << " to query\n";
      _componentPoolsArrays.push_back(newComponentPoolsArray);
      return true;
    }
    return false;
  }

  std::vector<std::array<C::IComponentPool *, sizeof...(Ts)>> &getComponentPoolsArrays()
  {
    return _componentPoolsArrays;
  }

  using TMapLambda = std::function<void(typename Ts::Types &...)>;

  // Lambda for mapping over the components' chunks
  using TCMapLambda = std::function<void(typename Ts::VTypes &...)>;

  // Lambda for mapping over the components' pools (Ts)
  using TPMapLambda = std::function<void(Ts *...)>;

  template<typename... TOthers>
  using TCrossLambda = std::function<void(typename Ts::Types &..., typename TOthers::Types &...)>;

  void map(EntityManager &eM, TMapLambda f)
  {
    eM.initializeQuery(*this);
    map(f);
  }

  void cMap(EntityManager &eM, TCMapLambda f)
  {
    eM.initializeQuery(*this);
    cMap(f);
  }

  void pMap(EntityManager &eM, TPMapLambda f)
  {
    eM.initializeQuery(*this);
    pMap(f);
  }

  template<typename... TOthers> void cross(EntityManager &eM, TCrossLambda<TOthers...> f)
  {
    Query<TOthers...> otherQuery;
    eM.initializeQuery(*this);
    eM.initializeQuery(otherQuery);
    cross(otherQuery, f);
  }

  void map(TMapLambda f,
    TCMapLambda startCB = nullptr,
    TCMapLambda endCB = nullptr,
    TPMapLambda pMapStartCB = nullptr,
    TPMapLambda pMapEndCB = nullptr)
  {
    cMap(
      [f, startCB, endCB](typename Ts::VTypes &...componentVectors) {
        if (startCB) { startCB(componentVectors...); }

        size_t vectorSize = std::get<0>(std::get<0>(std::tie(componentVectors...))).size();
        auto vectorTuple = std::tie(componentVectors...);
        using OuterIndices = std::make_index_sequence<std::tuple_size<decltype(vectorTuple)>::value>;
        for (size_t i = 0; i < vectorSize; ++i) {
          auto refTuple = _getReferencesAtIndex(i, vectorTuple, OuterIndices{});
          _callInnerMapOperate(f, refTuple, std::make_index_sequence<std::tuple_size<decltype(refTuple)>::value>{});
        }

        if (endCB) { endCB(componentVectors...); }
      },
      pMapStartCB,
      pMapEndCB);
  }

  void cMap(TCMapLambda f, TPMapLambda startCB = nullptr, TPMapLambda endCB = nullptr)
  {
    pMap([f, startCB, endCB](Ts *...componentPools) {
      if (startCB) { startCB(componentPools...); }

      size_t chunkCount = getFirstArg(componentPools...)->chunkCount();
      for (size_t i = 0; i < chunkCount; i++) {
        auto componentPoolsTuple =
          std::apply([i](auto &...pools) { return std::make_tuple(dynamic_cast<Ts *>(pools)->getRawStdVectors(i)...); },
            std::tie(componentPools...));

        std::apply([f](auto &...componentPools) { f(componentPools...); }, componentPoolsTuple);
      }

      if (endCB) { endCB(componentPools...); }
    });
  }

  void pMap(TPMapLambda f)
  {
    for (auto &componentPools : _componentPoolsArrays) {
      std::apply([&f](auto &...pools) { f(dynamic_cast<Ts *>(pools)...); }, componentPools);
    }
  }

  template<typename... TOthers> void cross(Query<TOthers...> &otherQuery, TCrossLambda<TOthers...> f)
  {
    auto &otherComponentPoolsArrays = otherQuery.getComponentPoolsArrays();

    std::cout << "\nRunning Cross QUERY\n";
    for (size_t i = 0; i < _componentPoolsArrays.size(); i++) {
      for (size_t j = 0; j < otherComponentPoolsArrays.size(); j++) {
        size_t chunkCount1 = _componentPoolsArrays[i][0]->chunkCount();
        size_t chunkCount2 = otherComponentPoolsArrays[j][0]->chunkCount();
        for (size_t k = 0; k < chunkCount1; k++) {
          for (size_t l = 0; l < chunkCount2; l++) {
            _cross<TOthers...>(f, _componentPoolsArrays[i], otherComponentPoolsArrays[j], k, l);
          }
          std::cout << "next chunk\n";
        }
      }
    }
  }

protected:
  template<typename... TOthers>
  constexpr static void _cross(TCrossLambda<TOthers...> f,
    std::array<C::IComponentPool *, sizeof...(Ts)> &componentPools1,
    std::array<C::IComponentPool *, sizeof...(TOthers)> &componentPools2,
    size_t index1,
    size_t index2)
  {
    auto componentPoolsTuple1 = std::apply(
      [index1](auto &...pools) { return std::make_tuple(dynamic_cast<Ts *>(pools)->getRawStdVectors(index1)...); },
      componentPools1);
    auto componentPoolsTuple2 = std::apply(
      [index2](auto &...pools) { return std::make_tuple(dynamic_cast<TOthers *>(pools)->getRawStdVectors(index2)...); },
      componentPools2);

    std::apply(
      [f, &componentPoolsTuple1, &componentPoolsTuple2](auto &...componentPools1) {
        std::apply(
          [f, &componentPools1...](
            auto &...componentPools2) { _crossOperate<TOthers...>(f, componentPools1..., componentPools2...); },
          componentPoolsTuple2);
      },
      componentPoolsTuple1);
  }

  template<typename... TOthers>
  constexpr static inline void _crossOperate(TCrossLambda<TOthers...> f,
    typename Ts::VTypes &...componentVectors1,
    typename TOthers::VTypes &...componentVectors2)
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
        _callInnerCrossOperate<TOthers...>(f,
          refTuple1,
          refTuple2,
          std::make_index_sequence<std::tuple_size<decltype(refTuple1)>::value>{},
          std::make_index_sequence<std::tuple_size<decltype(refTuple2)>::value>{});
      }
      std::cout << " ";
    }
    std::cout << "\n";
  }

  // Common utils templated constexpr functions to unpack tuples
  template<std::size_t... Is, typename... Vectors>
  constexpr static inline auto
    _getInnerReferencesAtIndex(size_t i, std::tuple<Vectors...> &vectorTuple, std::index_sequence<Is...> /*unused*/)
  {
    return std::make_tuple(std::ref(std::get<Is>(vectorTuple)[i])...);
  }

  template<std::size_t... OuterIs, typename... InnerTuples>
  constexpr static inline auto
    _getReferencesAtIndex(size_t i, std::tuple<InnerTuples...> &outerTuple, std::index_sequence<OuterIs...> /*unused*/)
  {
    return std::make_tuple(_getInnerReferencesAtIndex(i,
      std::get<OuterIs>(outerTuple),
      std::make_index_sequence<std::tuple_size<std::decay_t<decltype(std::get<OuterIs>(outerTuple))>>::value>{})...);
  }

  // Inners constexpr for each accessors
  template<typename Tuple, std::size_t... Is>
  constexpr static inline void
    _callInnerMapOperate(TMapLambda f, Tuple &refTuple, std::index_sequence<Is...> /*unused*/)
  {
    f(std::get<Is>(refTuple)...);
  }

  template<typename... TOthers, typename Tuple1, typename Tuple2, std::size_t... I1s, std::size_t... I2s>
  constexpr static inline void _callInnerCrossOperate(TCrossLambda<TOthers...> f,
    Tuple1 &refTuple1,
    Tuple2 &refTuple2,
    std::index_sequence<I1s...> /*unused*/,
    std::index_sequence<I2s...> /*unused*/)
  {
    f(std::get<I1s>(refTuple1)..., std::get<I2s>(refTuple2)...);
  }

  std::vector<std::array<C::IComponentPool *, sizeof...(Ts)>> _componentPoolsArrays;
};

// Partial specialization for when a tuple is passed
template<typename... Ts> class Query<std::tuple<Ts...>> : public Query<Ts...>
{
public:
  // Inherit everything from the base class Query<Ts...>
  using Query<Ts...>::Query;// Inheriting constructors as well (if needed)

  // Now this class behaves exactly as Query<Ts...> but accepts a tuple
};

}// namespace ECS::S
