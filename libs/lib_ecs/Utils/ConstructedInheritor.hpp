/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#pragma once

#include <tuple>
#include <utility>

#include <functional> // For std::reference_wrapper
template<typename... TArchetypePools>
struct TupConstructedInheritor : TArchetypePools... {
    explicit TupConstructedInheritor(std::tuple<TArchetypePools *...> pointers):
        TArchetypePools(*std::get<TArchetypePools *>(pointers))...
    {
    }
};
