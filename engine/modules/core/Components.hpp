#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"

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
