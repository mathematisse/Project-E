/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "Components/IComponentPool.hpp"
#include "Entities/IEntityPool.hpp"
#include "Systems/ISystem.hpp"
#include <vector>
#include <string>
#include <iostream>


    namespace ECS::Systems
    {

        // Helper metafunction to check if all types are derived from Base
        template <typename... Ts>
        struct all_derived_from_base;

        template <typename T>
        struct all_derived_from_base<T> : std::is_base_of<Components::IComponentPool, T> {};

        template <typename T, typename... Ts>
        struct all_derived_from_base<T, Ts...> : std::conjunction<std::is_base_of<Components::IComponentPool, T>, all_derived_from_base<Ts...>> {};


        template <typename... Ts>
        class ASystem : public ISystem
        {
            // Enable this class only if all types in Ts are derived from Base
            static_assert(all_derived_from_base<Ts...>::value, "All types must be derived from IComponentPool");
        public:
            explicit ASystem(bool isParallel) : _isParallel(isParallel) {}
            ~ASystem() override = default;
            ASystem(const ASystem &other) = default;
            ASystem(ASystem &&other) = default;
            ASystem &operator=(const ASystem &other) = default;
            ASystem &operator=(ASystem &&other) = default;
            [[nodiscard]] bool getIsParallel() const { return _isParallel; }

            bool tryAddEntityPool(Entities::IEntityPool* entityPool) override {
                std::vector<Components::IComponentPool *> componentPools = entityPool->getComponentPools();
                
                std::array<const std::string, sizeof...(Ts)> componentNames = {Ts::componentName...};
                std::array<Components::IComponentPool *, sizeof...(Ts)> newComponentPoolsArray = {nullptr};

                size_t i = 0;

                for (auto & componentPool : componentPools) {
                    for (auto & componentName : componentNames) {
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
                    std::cout << "Added entity pool " << entityPool->getEntityName() << " to system\n";
                    _componentPoolsArrays.push_back(newComponentPoolsArray);
                    return true;
                }
                return false;
            }

            void run() override {
                // print the name of the class
                std::cout << "Running system " << typeid(*this).name() << "\n";
                for (auto &componentPools : _componentPoolsArrays) {
                    size_t chunkCount = componentPools[0]->chunkCount();
                    std::cout << "Running step with chunk count: " << chunkCount << "\n";
                    for (size_t i = 0; i < chunkCount; i++) {
                        auto componentPoolsTuple = std::apply([i](auto&... pools) {
                            return std::make_tuple(dynamic_cast<Ts*>(pools)->getRawStdVectors(i)...);
                        }, componentPools);

                        std::apply([this](auto &... componentPools) {
                            operate(componentPools...);
                        }, componentPoolsTuple);
                   }
                }
            }

            template <std::size_t... Is, typename... Vectors>
            constexpr inline auto getInnerReferencesAtIndex(size_t i, std::tuple<Vectors...>& vectorTuple, std::index_sequence<Is...> /*unused*/) {
                return std::make_tuple(std::ref(std::get<Is>(vectorTuple)[i])...);
            }

            template <std::size_t... OuterIs, typename... InnerTuples>
            constexpr inline auto getReferencesAtIndex(size_t i, std::tuple<InnerTuples...>& outerTuple, std::index_sequence<OuterIs...> /*unused*/) {
                return std::make_tuple(getInnerReferencesAtIndex(i, std::get<OuterIs>(outerTuple), std::make_index_sequence<std::tuple_size<std::decay_t<decltype(std::get<OuterIs>(outerTuple))>>::value>{})...);
            }

            template <typename Tuple, std::size_t... Is>
            constexpr inline void callInnerOperate(Tuple& refTuple, std::index_sequence<Is...> /*unused*/) {
                innerOperate(std::get<Is>(refTuple)...);
            }

            void operate(typename Ts::VTypes&... componentVectors) {
                size_t vectorSize = std::get<0>(std::get<0>(std::tie(componentVectors...))).size();

                auto vectorTuple = std::tie(componentVectors...);

                using OuterIndices = std::make_index_sequence<std::tuple_size<decltype(vectorTuple)>::value>;

                std::cout << "v[" << vectorSize << "] = {";
                for (size_t i = 0; i < vectorSize; ++i) {
                    auto refTuple = getReferencesAtIndex(i, vectorTuple, OuterIndices{});

                    callInnerOperate(refTuple, std::make_index_sequence<std::tuple_size<decltype(refTuple)>::value>{});
                    std::cout << (i == vectorSize - 1 ? "" : ", ");

                }
                //reset
                std::cout << "\033[0m";
                std::cout << "}\n";
            }


            virtual inline void innerOperate(typename Ts::Types&... componentPools) = 0;

        protected:
            bool _isParallel;

            std::vector<std::array<Components::IComponentPool *, sizeof...(Ts)>> _componentPoolsArrays;
        };

    }
