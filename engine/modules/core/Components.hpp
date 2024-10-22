#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"

namespace ECS::C {

DECLARE_COMPONENT(
    Timer,
    float // ellapsed_seconds
);

}