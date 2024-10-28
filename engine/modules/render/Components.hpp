/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Render ecs module
*/

#pragma once

#include "lib_ecs/Core/ComponentMaker.hpp"
#include <cstdint>

DECLARE_COMPONENT(
    Sprite,
    size_t // textureId
);

DECLARE_COMPONENT(
    AnimatedSprite,
    size_t, // textureId
    std::uint8_t, // nbFrames
    float, // startPosition
    float // animationSpeed
);
