/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "lib_ecs/Systems/ExecutionTypes.hpp"
#include "lib_ecs/Systems/ISystem.hpp"
#include "lib_ecs/Systems/ASystemTree.hpp"

namespace ECS::S {

class ASystem : public ISystem {
public:
    explicit ASystem(SystemExecutionType execType = SERIAL_SYSTEM_EXECUTION):
        _execType(execType)
    {
    }
    ~ASystem() override = default;
    [[nodiscard]] SystemExecutionType getExecutionType() const { return _execType; };
    bool tryAddEntityPool(E::IArchetypePool *entityPool) override = 0;
    void getRunStepData(ASystemTree &sysTree) override {};

protected:
    SystemExecutionType _execType;
};

class ADeltaTimeSystem : virtual public ASystem {
public:
    explicit ADeltaTimeSystem(SystemExecutionType execType = SERIAL_SYSTEM_EXECUTION):
        ASystem(execType)
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
