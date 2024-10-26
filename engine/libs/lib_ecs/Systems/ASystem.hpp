/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "lib_ecs/Systems/ISystem.hpp"
#include "lib_ecs/Systems/ASystemTree.hpp"

namespace ECS::S {

class ASystem : virtual public ISystem {
public:
    explicit ASystem(bool isParallel = false):
        _isParallel(isParallel)
    {
    }
    ~ASystem() override = default;
    [[nodiscard]] bool getIsParallel() const { return _isParallel; };
    bool tryAddEntityPool(E::IEntityPool *entityPool) override = 0;
    void getRunStepData(ASystemTree &sysTree) override {};

protected:
    bool _isParallel;
};

class ADeltaTimeSystem : virtual public ASystem {
public:
    explicit ADeltaTimeSystem(bool isParallel = false):
        ASystem(isParallel)
    {
    }
    ~ADeltaTimeSystem() override = default;
    void getRunStepData(ASystemTree &sysTree) override { _deltaTime = sysTree.getDeltaTime(); };

    [[nodiscard]] float getDeltaTime() const { return _deltaTime; }
    void setDeltaTime(float newDeltaTime) { _deltaTime = newDeltaTime; }

protected:
    float _deltaTime = 0.0F;
};

} // namespace ECS::S
