#pragma once

#include "lib_ecs/Core/ComponentMaker.hpp"

DECLARE_COMPONENT(
    Timer,
    float // ellapsed_seconds
);

DECLARE_COMPONENT(
    Color,
    unsigned char, // r
    unsigned char, // g
    unsigned char, // b
    unsigned char // a
)
