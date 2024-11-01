#pragma once

#include "lib_ecs/Core/ComponentMaker.hpp"

DECLARE_COMPONENT(
    Position,
    float, // x
    float // y
);

DECLARE_COMPONENT(
    Size,
    float, // x
    float // y
);

DECLARE_COMPONENT(
    Rotation,
    float // degres
);

DECLARE_COMPONENT(
    Velocity,
    float, // x
    float // y
);

DECLARE_COMPONENT(
    Acceleration,
    float, // x
    float // y
);
