/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ISystem
*/

#pragma once

#include "lib_ecs/Entities/IEntityPool.hpp"

namespace ECS::S {

class SystemTree;

class ISystem {
public:
    ISystem() = default;
    virtual ~ISystem() = default;
    ISystem(const ISystem &other) = default;
    ISystem(ISystem &&other) = default;
    ISystem &operator=(const ISystem &other) = default;
    ISystem &operator=(ISystem &&other) = default;
    virtual bool tryAddEntityPool(E::IEntityPool *entityPool) = 0;
    virtual void getRunStepData(SystemTree &sysTree) = 0; // get necessary data for that run
    virtual void run() = 0;
};
} // namespace ECS::S
