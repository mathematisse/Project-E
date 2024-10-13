/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "Entities/IEntityPool.hpp"
#include "Systems/ASystem.hpp"
#include "Systems/Query.hpp"


namespace ECS::S {

template<typename Tuple1, typename Tuple2> class ADualSystem;

template<typename... T1s, typename... T2s> class ADualSystem<std::tuple<T1s...>, std::tuple<T2s...>> : public ASystem
{

public:
  explicit ADualSystem(bool isParallel) : ASystem(isParallel) {}
  ~ADualSystem() override = default;

  ADualSystem(const ADualSystem &other) = default;
  ADualSystem(ADualSystem &&other) = default;
  ADualSystem &operator=(const ADualSystem &other) = default;
  ADualSystem &operator=(ADualSystem &&other) = default;

  bool tryAddEntityPool(E::IEntityPool *entityPool) override
  {
    auto q1 = _query1.tryAddEntityPool(entityPool);
    auto q2 = _query2.tryAddEntityPool(entityPool);
    return q1 || q2;
  }

  void run() override
  {
    _query1.cross(_query2, [this](auto &...componentPools1, auto &...componentPools2) {
      _innerOperate(componentPools1..., componentPools2...);
    });
  }

protected:
  Query<T1s...> _query1;
  Query<T2s...> _query2;

  virtual inline void _innerOperate(typename T1s::Types &...componentPools1,
    typename T2s::Types &...componentPools2) = 0;
};
}// namespace ECS::S
