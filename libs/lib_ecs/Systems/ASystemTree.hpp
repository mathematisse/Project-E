/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base SystemTree
*/

#pragma once

namespace ECS::S {

class ASystemTree {
public:
    [[nodiscard]] double getDeltaTime() const { return _deltaTime; }
    void setDeltaTime(double newDeltaTime) { _deltaTime = newDeltaTime; }

protected:
    double _deltaTime = 0.0F;
};
} // namespace ECS::S
