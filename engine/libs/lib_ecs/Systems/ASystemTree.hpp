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
    [[nodiscard]] float getDeltaTime() const { return _deltaTime; }
    void setDeltaTime(float newDeltaTime) { _deltaTime = newDeltaTime; }

protected:
    float _deltaTime = 0.0F;
};
} // namespace ECS::S
