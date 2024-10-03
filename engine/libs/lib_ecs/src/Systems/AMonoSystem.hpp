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
#include <vector>
#include <string>
#include <iostream>


    namespace ECS::Systems
    {
        template <typename... Ts>
        class AMonoSystem : public ASystem
        {
            // Enable this class only if all types in Ts are derived from Base
            static_assert(all_derived_from_base<Ts...>::value, "All types must be derived from IComponentPool");
        public:
            explicit AMonoSystem(bool isParallel) : ASystem(isParallel) {}
            ~AMonoSystem() override = default;

            AMonoSystem(const AMonoSystem &other) = default;
            AMonoSystem(AMonoSystem &&other) = default;
            AMonoSystem &operator=(const AMonoSystem &other) = default;
            AMonoSystem &operator=(AMonoSystem &&other) = default;

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
                std::cout << "\nRunning system " << typeid(*this).name() << "\n";
                for (auto &componentPools : _componentPoolsArrays) {
                    size_t chunkCount = componentPools[0]->chunkCount();
                    for (size_t i = 0; i < chunkCount; i++) {
                        _run(componentPools, i);
                    }
                }
            }

        private:
            void _run(std::array<Components::IComponentPool *, sizeof...(Ts)>& componentPools, size_t index) {
                auto componentPoolsTuple = std::apply([index](auto&... pools) {
                    return std::make_tuple(dynamic_cast<Ts*>(pools)->getRawStdVectors(index)...);
                }, componentPools);

                std::apply([this](auto &... componentPools) {
                    _operate(componentPools...);
                }, componentPoolsTuple);
            }

        protected:

            void _operate(typename Ts::VTypes&... componentVectors) {
                size_t vectorSize = std::get<0>(std::get<0>(std::tie(componentVectors...))).size();

                auto vectorTuple = std::tie(componentVectors...);

                using OuterIndices = std::make_index_sequence<std::tuple_size<decltype(vectorTuple)>::value>;

                for (size_t i = 0; i < vectorSize; ++i) {
                    auto refTuple = _getReferencesAtIndex(i, vectorTuple, OuterIndices{});
                    _callInnerOperate(refTuple, std::make_index_sequence<std::tuple_size<decltype(refTuple)>::value>{});
                }
                std::cout << "\n";
            }


            virtual inline void _innerOperate(typename Ts::Types&... componentPools) = 0;

            template <typename Tuple, std::size_t... Is>
            constexpr inline void _callInnerOperate(Tuple& refTuple, std::index_sequence<Is...> /*unused*/) {
                _innerOperate(std::get<Is>(refTuple)...);
            }
            
            std::vector<std::array<Components::IComponentPool *, sizeof...(Ts)>> _componentPoolsArrays;
        };
    } // namespace ECS::Systems

