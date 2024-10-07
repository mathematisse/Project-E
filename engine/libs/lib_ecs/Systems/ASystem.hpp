/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "lib_ecs/Systems/ISystem.hpp"

namespace ECS::S {
class ASystem : public ISystem {
public:
    explicit ASystem(bool isParallel):
        _isParallel(isParallel)
    {
    }
    ~ASystem() override = default;
    ASystem(const ASystem &other) = default;
    ASystem(ASystem &&other) = default;
    ASystem &operator=(const ASystem &other) = default;
    ASystem &operator=(ASystem &&other) = default;
    [[nodiscard]] bool getIsParallel() const { return _isParallel; }
    bool tryAddEntityPool(E::IEntityPool *entityPool) override = 0;

protected:
    bool _isParallel;
};

} // namespace ECS::S
