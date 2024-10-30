/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for Tower entity declarations
*/

#pragma once

#include <raylib.h>
#include "lib_ecs/Core/ComponentMaker.hpp"

#define TOWER_COUNT 9

static const std::vector<std::string> tower_names {"None", "Bomb", "Archer", "Wizard"};

static const std::vector<std::vector<size_t>> tower_range {
    {0, 0, 0}, {300, 300, 300}, {300, 350, 400}, {250, 300, 350}
};

enum class TowerType {
    NONE = 0,
    BOMB = 1,
    ARCHER = 2,
    WIZARD = 3,
};

struct tower_info {
    size_t id;
    size_t level;
    TowerType type;
    Vector2 pos;
    float delay;
};

DECLARE_COMPONENT(Type, TowerType);
DECLARE_COMPONENT(Level, size_t);
DECLARE_COMPONENT(ID, size_t);
DECLARE_COMPONENT(Dest, Vector2);
DECLARE_COMPONENT(Score, size_t);
DECLARE_COMPONENT(Health, int);