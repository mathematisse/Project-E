/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#pragma once

#include "Components.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "raylib.h"

namespace ECS::S {

class ShowInfoSystem : public S::AMonoSystem<C::Type::Pool, C::Health::Pool> {
public:
    explicit ShowInfoSystem(Camera2D &camera);
    ~ShowInfoSystem() override = default;

    Camera2D &camera;
    bool one_time = false;

protected:
    void _innerOperate(C::Type::Pool::Types &ctype, C::Health::Pool::Types &chealth) override;
};

} // namespace ECS::S
