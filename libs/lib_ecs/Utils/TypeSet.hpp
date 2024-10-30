/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include <tuple>
#include <type_traits>

// Helper metafunction to check if a type is in a tuple
template<typename T, typename Tuple>
struct is_in_tuple;

template<typename T>
struct is_in_tuple<T, std::tuple<>> : std::false_type { };

template<typename T, typename U, typename... Ts>
struct is_in_tuple<T, std::tuple<U, Ts...>> : is_in_tuple<T, std::tuple<Ts...>> { };

template<typename T, typename... Ts>
struct is_in_tuple<T, std::tuple<T, Ts...>> : std::true_type { };

// Helper metafunction to add a type to a tuple if it's not already present
template<typename T, typename Tuple>
struct add_type_if_not_in_tuple;

template<typename T, typename... Ts>
struct add_type_if_not_in_tuple<T, std::tuple<Ts...>> {
    using type = std::conditional_t<
        is_in_tuple<T, std::tuple<Ts...>>::value, std::tuple<Ts...>, std::tuple<Ts..., T>>;
};

// TypeSet to create a set of unique types
template<typename... Types>
struct TypeSet;

template<>
struct TypeSet<> {
    using type = std::tuple<>;
};

template<typename T, typename... Rest>
struct TypeSet<T, Rest...> {
private:
    using RestSet = typename TypeSet<Rest...>::type;

public:
    using type = typename add_type_if_not_in_tuple<T, RestSet>::type;
};

// TupTypeSet to extract unique types from a tuple
template<typename Tuple>
struct TupTypeSet;

template<typename... Types>
struct TupTypeSet<std::tuple<Types...>> {
    using type = typename TypeSet<Types...>::type;
};

#define TUPSET(...) typename TupTypeSet<__VA_ARGS__>::type