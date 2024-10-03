/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "Components/IComponentPool.hpp"
#include "Systems/ISystem.hpp"


    namespace ECS::Systems
    {
        class ASystem : public ISystem
        {
        public:
            explicit ASystem(bool isParallel) : _isParallel(isParallel) {}
            ~ASystem() override = default;
            ASystem(const ASystem &other) = default;
            ASystem(ASystem &&other) = default;
            ASystem &operator=(const ASystem &other) = default;
            ASystem &operator=(ASystem &&other) = default;
            [[nodiscard]] bool getIsParallel() const { return _isParallel; }


        protected:
            template <std::size_t... Is, typename... Vectors>
            constexpr inline auto _getInnerReferencesAtIndex(size_t i, std::tuple<Vectors...>& vectorTuple, std::index_sequence<Is...> /*unused*/) {
                return std::make_tuple(std::ref(std::get<Is>(vectorTuple)[i])...);
            }

            template <std::size_t... OuterIs, typename... InnerTuples>
            constexpr inline auto _getReferencesAtIndex(size_t i, std::tuple<InnerTuples...>& outerTuple, std::index_sequence<OuterIs...> /*unused*/) {
                return std::make_tuple(_getInnerReferencesAtIndex(i, std::get<OuterIs>(outerTuple), std::make_index_sequence<std::tuple_size<std::decay_t<decltype(std::get<OuterIs>(outerTuple))>>::value>{})...);
            }
            bool _isParallel;
        };


        // Helper metafunction to check if all types are derived from Base
        template <typename... Ts>
        struct all_derived_from_base;

        template <typename T>
        struct all_derived_from_base<T> : std::is_base_of<Components::IComponentPool, T> {};

        template <typename T, typename... Ts>
        struct all_derived_from_base<T, Ts...> : std::conjunction<std::is_base_of<Components::IComponentPool, T>, all_derived_from_base<Ts...>> {};
    } // namespace ECS::Systems

