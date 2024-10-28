/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntity
*/

#pragma once

#include "lib_ecs/Core/ArchetypeMaker.hpp"
#include "lib_ecs/Core/RawArchComponents.hpp"
#include "lib_ecs/Entities/Aspect.hpp"
#include <tuple>

namespace ECS::E {

using EntrPtrAspect = Aspect<std::tuple<ECS::C::ArchEntityPoolId>>;

} // namespace ECS::E

DECLARE_ARCHETYPE(EntityPtr, EntrPtrAspect)