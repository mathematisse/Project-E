/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "lib_ecs/Components/IComponentPool.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/IQuery.hpp"
#include "lib_log/log.hpp"
#include <functional>
#include <string>
#include <thread>
#include <vector>
#include <array>

template<typename First, typename... Rest>
constexpr inline First getFirstArg(First first, Rest... /*unused*/)
{
    return first;
}

namespace ECS::S {

// Helper metafunction to check if all types are derived from Base
template<typename... Ts>
struct all_derived_from_base;

template<typename T>
struct all_derived_from_base<T> : std::is_base_of<C::IComponentPool, T> { };

template<typename T, typename... Ts>
struct all_derived_from_base<T, Ts...>
    : std::conjunction<std::is_base_of<C::IComponentPool, T>, all_derived_from_base<Ts...>> { };

template<typename... Ts>
class Query : public IQuery {

    // Enable this class only if all types in Ts are derived from Base
    static_assert(
        all_derived_from_base<Ts...>::value, "All types must be derived from IComponentPool"
    );

public:
    Query() = default;
    ~Query() override = default;

    bool tryAddEntityPool(E::IArchetypePool *entityPool) override
    {
        std::vector<C::IComponentPool *> componentPools = entityPool->getVecComponentPools();

        std::array<const std::string, sizeof...(Ts)> componentNames = {Ts::componentName...};
        std::array<C::IComponentPool *, sizeof...(Ts)> newComponentPoolsArray = {nullptr};

        size_t i = 0;

        for (auto &componentName : componentNames) {
            for (auto &componentPool : componentPools) {
                if (componentPool->getComponentName() == componentName) {
                    if (i == sizeof...(Ts)) {
                        return false;
                    }
                    newComponentPoolsArray[i] = componentPool;
                    i++;
                }
            }
        }
        if (i == sizeof...(Ts)) {
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

    void map(EntityManager &eM, TMapLambda f, bool isParallel = false)
    {
        eM.initializeQuery(*this);
        map(f, isParallel);
    }

    void cMap(EntityManager &eM, TCMapLambda f, bool isParallel = false)
    {
        eM.initializeQuery(*this);
        cMap(f, isParallel);
    }

    void pMap(EntityManager &eM, TPMapLambda f, bool isParallel = false)
    {
        eM.initializeQuery(*this);
        pMap(f, isParallel);
    }

    void selfCross(EntityManager &eM, TCrossLambda<Ts...> f, bool isParallel = false)
    {
        eM.initializeQuery(*this);
        cross<Ts...>(*this, f, true, isParallel);
    }

    template<typename... TOthers>
    void cross(EntityManager &eM, TCrossLambda<TOthers...> f, bool isParallel = false)
    {
        Query<TOthers...> otherQuery;
        eM.initializeQuery(*this);
        eM.initializeQuery(otherQuery);
        cross(otherQuery, f, false, isParallel);
    }

    void
    map(TMapLambda f, bool isParallel = false, TCMapLambda startCB = nullptr,
        TCMapLambda endCB = nullptr, TPMapLambda pMapStartCB = nullptr,
        TPMapLambda pMapEndCB = nullptr)
    {
        cMap(
            [f, startCB, endCB](typename Ts::VTypes &...componentVectors) {
                if (startCB) {
                    startCB(componentVectors...);
                }

                size_t vectorSize = std::get<0>(std::get<0>(std::tie(componentVectors...))).size();
                auto vectorTuple = std::tie(componentVectors...);
                using OuterIndices =
                    std::make_index_sequence<std::tuple_size<decltype(vectorTuple)>::value>;
                for (size_t i = 0; i < vectorSize; ++i) {
                    auto refTuple = _getReferencesAtIndex(i, vectorTuple, OuterIndices {});
                    _callInnerMapOperate(
                        f, refTuple,
                        std::make_index_sequence<std::tuple_size<decltype(refTuple)>::value> {}
                    );
                }

                if (endCB) {
                    endCB(componentVectors...);
                }
            },
            isParallel, pMapStartCB, pMapEndCB
        );
    }

    void cMap(
        TCMapLambda f, bool isParallel = false, TPMapLambda startCB = nullptr,
        TPMapLambda endCB = nullptr
    )
    {
        if (isParallel) {
            // Run the pMap function in parallel using multiple threads
            std::vector<std::thread> threads;

            pMap(
                [f](Ts *...componentPools) {
                    size_t chunkCount = getFirstArg(componentPools...)->chunkCount();
                    for (size_t i = 0; i < chunkCount; i++) {
                        auto componentPoolsTuple = std::apply(
                            [i](auto &...pools) {
                                return std::make_tuple(static_cast<Ts *>(pools)->getRawStdVectors(i
                                )...);
                            },
                            std::tie(componentPools...)
                        );

                        std::apply(
                            [f](auto &...componentPools) {
                                f(componentPools...);
                            },
                            componentPoolsTuple
                        );
                    }
                },
                true
            );

            // Wait for all threads to finish
            for (auto &t : threads) {
                if (t.joinable()) {
                    t.join();
                }
            }

        } else {
            // Default behavior: run pMap sequentially
            pMap(
                [f](Ts *...componentPools) {
                    size_t chunkCount = getFirstArg(componentPools...)->chunkCount();
                    for (size_t i = 0; i < chunkCount; i++) {
                        auto componentPoolsTuple = std::apply(
                            [i](auto &...pools) {
                                return std::make_tuple(static_cast<Ts *>(pools)->getRawStdVectors(i
                                )...);
                            },
                            std::tie(componentPools...)
                        );

                        std::apply(
                            [f](auto &...componentPools) {
                                f(componentPools...);
                            },
                            componentPoolsTuple
                        );
                    }
                },
                false
            );
        }
    }

    void pMap(TPMapLambda f, bool isParallel = false)
    {
        if (isParallel) {
            // Run the pMap function in parallel using multiple threads
            std::vector<std::thread> threads;

            for (auto &componentPools : _componentPoolsArrays) {
                threads.emplace_back([&f, &componentPools]() {
                    std::apply(
                        [&f](auto &...pools) {
                            f(static_cast<Ts *>(pools)...);
                        },
                        componentPools
                    );
                });
            }

            // Wait for all threads to finish
            for (auto &t : threads) {
                if (t.joinable()) {
                    t.join();
                }
            }

        } else {
            // Default behavior: run pMap sequentially
            for (auto &componentPools : _componentPoolsArrays) {
                std::apply(
                    [&f](auto &...pools) {
                        f(static_cast<Ts *>(pools)...);
                    },
                    componentPools
                );
            }
        }
    }

    template<typename... TOthers>
    void cross(
        Query<TOthers...> &otherQuery, TCrossLambda<TOthers...> f, bool selfCross = false,
        bool isParallel = false
    )
    {
        if (isParallel) {
            // Run the cross function in parallel using multiple threads
            std::vector<std::thread> threads;

            for (auto &componentPools1 : _componentPoolsArrays) {
                threads.emplace_back([&otherQuery, &f, &componentPools1, selfCross]() {
                    for (auto &componentPools2 : otherQuery.getComponentPoolsArrays()) {
                        size_t chunkCount1 = componentPools1[0]->chunkCount();
                        size_t chunkCount2 = componentPools2[0]->chunkCount();
                        for (size_t k = 0; k < chunkCount1; k++) {
                            for (size_t l = 0; l < chunkCount2; l++) {
                                _cross<TOthers...>(
                                    f, componentPools1, componentPools2, k, l, selfCross
                                );
                            }
                        }
                    }
                });
            }

            // Wait for all threads to finish
            for (auto &t : threads) {
                if (t.joinable()) {
                    t.join();
                }
            }

        } else {
            // Default behavior: run cross sequentially
            for (auto &componentPools1 : _componentPoolsArrays) {
                for (auto &componentPools2 : otherQuery.getComponentPoolsArrays()) {
                    size_t chunkCount1 = componentPools1[0]->chunkCount();
                    size_t chunkCount2 = componentPools2[0]->chunkCount();
                    for (size_t k = 0; k < chunkCount1; k++) {
                        for (size_t l = 0; l < chunkCount2; l++) {
                            _cross<TOthers...>(
                                f, componentPools1, componentPools2, k, l, selfCross
                            );
                        }
                    }
                }
            }
        }
    }

    void selfCross(TCrossLambda<Ts...> f, bool isParallel)
    {
        cross<Ts...>(*this, f, true, isParallel);
    }

protected:
    template<typename... TOthers>
    constexpr static void _cross(
        TCrossLambda<TOthers...> f, std::array<C::IComponentPool *, sizeof...(Ts)> &componentPools1,
        std::array<C::IComponentPool *, sizeof...(TOthers)> &componentPools2, size_t index1,
        size_t index2, bool selfCross = false
    )
    {
        auto componentPoolsTuple1 = std::apply(
            [index1](auto &...pools) {
                return std::make_tuple(static_cast<Ts *>(pools)->getRawStdVectors(index1)...);
            },
            componentPools1
        );
        auto componentPoolsTuple2 = std::apply(
            [index2](auto &...pools) {
                return std::make_tuple(static_cast<TOthers *>(pools)->getRawStdVectors(index2)...);
            },
            componentPools2
        );

        std::apply(
            [selfCross, f, &componentPoolsTuple1, &componentPoolsTuple2](auto &...componentPools1) {
                std::apply(
                    [selfCross, f, &componentPools1...](auto &...componentPools2) {
                        if (selfCross) {
                            _selfCrossOperate<TOthers...>(
                                f, componentPools1..., componentPools2...
                            );
                        } else {
                            _crossOperate<TOthers...>(f, componentPools1..., componentPools2...);
                        }
                    },
                    componentPoolsTuple2
                );
            },
            componentPoolsTuple1
        );
    }

    template<typename... TOthers>
    constexpr static inline void _crossOperate(
        TCrossLambda<TOthers...> f, typename Ts::VTypes &...componentVectors1,
        typename TOthers::VTypes &...componentVectors2
    )
    {
        size_t vectorSize1 = std::get<0>(std::get<0>(std::tie(componentVectors1...))).size();
        size_t vectorSize2 = std::get<0>(std::get<0>(std::tie(componentVectors2...))).size();

        auto vectorTuple1 = std::tie(componentVectors1...);
        auto vectorTuple2 = std::tie(componentVectors2...);

        using OuterIndices1 =
            std::make_index_sequence<std::tuple_size<decltype(vectorTuple1)>::value>;
        using OuterIndices2 =
            std::make_index_sequence<std::tuple_size<decltype(vectorTuple2)>::value>;

        for (size_t i = 0; i < vectorSize1; ++i) {
            for (size_t j = 0; j < vectorSize2; ++j) {
                auto refTuple1 = _getReferencesAtIndex(i, vectorTuple1, OuterIndices1 {});
                auto refTuple2 = _getReferencesAtIndex(j, vectorTuple2, OuterIndices2 {});
                _callInnerCrossOperate<TOthers...>(
                    f, refTuple1, refTuple2,
                    std::make_index_sequence<std::tuple_size<decltype(refTuple1)>::value> {},
                    std::make_index_sequence<std::tuple_size<decltype(refTuple2)>::value> {}
                );
            }
        }
    }

    template<typename... TOthers>
    constexpr static inline void _selfCrossOperate(
        TCrossLambda<TOthers...> f, typename Ts::VTypes &...componentVectors1,
        typename TOthers::VTypes &...componentVectors2
    )
    {
        // here the size of the vectors is the same (self cross)
        size_t vectorSize1 = std::get<0>(std::get<0>(std::tie(componentVectors1...))).size();

        bool sameVecs = (void *) &std::get<0>(std::get<0>(std::tie(componentVectors1...))) ==
            (void *) &std::get<0>(std::get<0>(std::tie(componentVectors2...)));

        auto vectorTuple1 = std::tie(componentVectors1...);
        auto vectorTuple2 = std::tie(componentVectors2...);

        using OuterIndices1 =
            std::make_index_sequence<std::tuple_size<decltype(vectorTuple1)>::value>;
        using OuterIndices2 =
            std::make_index_sequence<std::tuple_size<decltype(vectorTuple2)>::value>;

        for (size_t i = 0; i < vectorSize1; ++i) {
            // note the j < i, to discard checking the same pair twice, as well as entities with
            // themselves
            for (size_t j = 0; j <= i; ++j) {
                if (sameVecs && i == j) {
                    continue;
                }
                auto refTuple1 = _getReferencesAtIndex(i, vectorTuple1, OuterIndices1 {});
                auto refTuple2 = _getReferencesAtIndex(j, vectorTuple2, OuterIndices2 {});
                _callInnerCrossOperate<TOthers...>(
                    f, refTuple1, refTuple2,
                    std::make_index_sequence<std::tuple_size<decltype(refTuple1)>::value> {},
                    std::make_index_sequence<std::tuple_size<decltype(refTuple2)>::value> {}
                );
            }
        }
    }

    // Common utils templated constexpr functions to unpack tuples
    template<std::size_t... Is, typename... Vectors>
    constexpr static inline auto _getInnerReferencesAtIndex(
        size_t i, std::tuple<Vectors...> &vectorTuple, std::index_sequence<Is...> /*unused*/
    )
    {
        return std::make_tuple(std::ref(std::get<Is>(vectorTuple)[i])...);
    }

    template<std::size_t... OuterIs, typename... InnerTuples>
    constexpr static inline auto _getReferencesAtIndex(
        size_t i, std::tuple<InnerTuples...> &outerTuple, std::index_sequence<OuterIs...> /*unused*/
    )
    {
        return std::make_tuple(_getInnerReferencesAtIndex(
            i, std::get<OuterIs>(outerTuple),
            std::make_index_sequence<
                std::tuple_size<std::decay_t<decltype(std::get<OuterIs>(outerTuple))>>::value> {}
        )...);
    }

    // Inners constexpr for each accessors
    template<typename Tuple, std::size_t... Is>
    constexpr static inline void
    _callInnerMapOperate(TMapLambda f, Tuple &refTuple, std::index_sequence<Is...> /*unused*/)
    {
        f(std::get<Is>(refTuple)...);
    }

    template<
        typename... TOthers, typename Tuple1, typename Tuple2, std::size_t... I1s,
        std::size_t... I2s>
    constexpr static inline void _callInnerCrossOperate(
        TCrossLambda<TOthers...> f, Tuple1 &refTuple1, Tuple2 &refTuple2,
        std::index_sequence<I1s...> /*unused*/, std::index_sequence<I2s...> /*unused*/
    )
    {
        f(std::get<I1s>(refTuple1)..., std::get<I2s>(refTuple2)...);
    }

    std::vector<std::array<C::IComponentPool *, sizeof...(Ts)>> _componentPoolsArrays;
};

// Partial specialization for when a tuple is passed
template<typename... Ts>
class Query<std::tuple<Ts...>> : public Query<Ts...> {
public:
    // Inherit everything from the base class Query<Ts...>
    using Query<Ts...>::Query; // Inheriting constructors as well (if needed)

    // Now this class behaves exactly as Query<Ts...> but accepts a tuple
};

} // namespace ECS::S
