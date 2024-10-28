/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs IQUery class
*/

#pragma once

#include "lib_ecs/Entities/IArchetypes.hpp"

namespace ECS::S {

class IQuery {
public:
    virtual ~IQuery() = default;
    virtual bool tryAddEntityPool(E::IArchetypePool *entityPool) = 0;
};

} // namespace ECS::S