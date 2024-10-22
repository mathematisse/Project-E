#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"

namespace ECS::C {

DECLARE_COMPONENT(
    Position,
    float, // x
    float // y
);

DECLARE_COMPONENT(
    Scale,
    float, // x
    float // y
);

DECLARE_COMPONENT(
    Rostation,
    float // degres
);

DECLARE_COMPONENT(
    Velocity,
    float, // x
    float, // y
    float // speed
);

DECLARE_COMPONENT(
    Acceleration,
    float, // x
    float // y
);

}