/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for DecorEntity entity declarations
*/

#pragma once

#include "lib_ecs/Core/ComponentMaker.hpp"

enum class GameEntityType {
    NONE,
    LPLAYER,
    PLAYER,
    ENEMY,
    BULLET,
    BULLET_ENNEMY,
    WALL,
    POWERUP,
    BACKGROUND,
    ENGINE
};

DECLARE_COMPONENT(Type, GameEntityType);

enum class WeaponType {
    BULLET,
    BIG_SHOT,
};

DECLARE_COMPONENT(CanShoot, char, float, float);
DECLARE_COMPONENT(Health, size_t);
DECLARE_COMPONENT(IsShooting, char);
DECLARE_COMPONENT(Weapon, WeaponType);