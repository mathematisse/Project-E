/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#pragma once

#include <cstdint>
#include <iostream>

// ---

namespace ECS::C {

using entity_pool_id_t = uint8_t;

using entity_status_t = int8_t;

enum EntityStatusEnum : entity_status_t {
    ENT_ERROR = -1,
    ENT_NONE = 0,
    ENT_NEEDS_INIT = 1,
    ENT_NEEDS_DESTROY = 2,
    ENT_ALIVE = 3,
    ENT_CHANGE_WEAPON = 4,
};
} // namespace ECS::C

inline std::ostream &operator<<(std::ostream &os, ECS::C::EntityStatusEnum status)
{
    switch (status) {
    case ECS::C::EntityStatusEnum::ENT_ERROR:
        os << "ENT_ERROR";
        break;
    case ECS::C::EntityStatusEnum::ENT_NONE:
        os << "ENT_NONE";
        break;
    case ECS::C::EntityStatusEnum::ENT_NEEDS_INIT:
        os << "ENT_NEEDS_INIT";
        break;
    case ECS::C::EntityStatusEnum::ENT_NEEDS_DESTROY:
        os << "ENT_NEEDS_DESTROY";
        break;
    case ECS::C::EntityStatusEnum::ENT_ALIVE:
        os << "ENT_ALIVE";
        break;
    default:
        os << "UNKNOWN";
        break;
    }
    return os;
}
