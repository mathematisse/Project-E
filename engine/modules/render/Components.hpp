/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Render ecs module
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"

DECLARE_COMPONENT(
    Sprite,
    size_t // textureId
);

DECLARE_COMPONENT(
    AnimatedSprite,
    size_t, // textureId
    float, // nbFrames
    float, // startPosition
    float // animationSpeed
);
