/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs IQUery class
*/

#pragma once

#include "lib_ecs/Entities/IEntityPool.hpp"

namespace ECS::S {

class IQuery {
public:
    virtual bool tryAddEntityPool(E::IEntityPool *entityPool) = 0;
};

} // namespace ECS::S