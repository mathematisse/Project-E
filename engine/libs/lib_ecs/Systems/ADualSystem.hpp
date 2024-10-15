/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "lib_ecs/Entities/IEntityPool.hpp"
#include "lib_ecs/Systems/ASystem.hpp"
#include "lib_ecs/Systems/Query.hpp"

// #include <chrono>

namespace ECS::S {

template<typename Tuple1, typename Tuple2>
class ADualSystem;

template<typename... T1s, typename... T2s>
class ADualSystem<std::tuple<T1s...>, std::tuple<T2s...>> : public ASystem {

public:
    explicit ADualSystem(bool isParallel):
        ASystem(isParallel)
    {
    }
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
        // auto now = std::chrono::high_resolution_clock::now();
        _query1.cross(
            _query2,
            [this](auto &...componentPools1, auto &...componentPools2) {
                _innerOperate(componentPools1..., componentPools2...);
            },
            _isParallel
        );

        // auto duration =
        // std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()
        // - now); std::cout << "DualSystem " << typeid(this).name() << " took " << duration.count()
        // << " microseconds\n";
    }

protected:
    Query<T1s...> _query1;
    Query<T2s...> _query2;

    virtual inline void _innerOperate(
        typename T1s::Types &...componentPools1, typename T2s::Types &...componentPools2
    ) = 0;
};

template<typename Tuple>
class ASelfDualSystem;

template<typename... Ts>
class ASelfDualSystem<std::tuple<Ts...>>
    : public ADualSystem<std::tuple<Ts...>, std::tuple<Ts...>> {
public:
    explicit ASelfDualSystem(bool isParallel):
        ADualSystem<std::tuple<Ts...>, std::tuple<Ts...>>(isParallel)
    {
    }
    ~ASelfDualSystem() override = default;

    ASelfDualSystem(const ASelfDualSystem &other) = default;
    ASelfDualSystem(ASelfDualSystem &&other) = default;
    ASelfDualSystem &operator=(const ASelfDualSystem &other) = default;
    ASelfDualSystem &operator=(ASelfDualSystem &&other) = default;

protected:
    void run() override
    {
        // auto now = std::chrono::high_resolution_clock::now();
        this->_query1.selfCross(
            [this](auto &...componentPools1, auto &...componentPools2) {
                _innerOperate(componentPools1..., componentPools2...);
            },
            this->_isParallel
        );

        // auto duration =
        // std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()
        // - now); std::cout << "SelfDualSystem " << typeid(this).name() << " took " <<
        // duration.count() << " microseconds\n";
    }

    virtual inline void _innerOperate(
        typename Ts::Types &...componentPools1, typename Ts::Types &...componentPools2
    ) = 0;
};

} // namespace ECS::S
