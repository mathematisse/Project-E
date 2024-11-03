/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#define MULTITHREADING true

namespace ECS::S {
enum NodeExecutionType {
    SERIAL_NODE_EXECUTION,
    PARALLEL_NODE_EXECUTION,
    NO_SYNC_NODE_EXECUTION
};

enum SystemExecutionType {
    SERIAL_SYSTEM_EXECUTION,
    PARALLEL_SYSTEM_EXECUTION
};
} // namespace ECS::S