/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IComponent
*/

#pragma once

namespace ECS::C {
/**
 * @class IComponent
 * @brief Abstract base class for all components in the ECS.
 *
 * The IComponent class provides a common interface for all components in the ECS.
 * It ensures that all derived components can be properly managed and destroyed.
 */
class IComponentRef {
public:
    /**
     * @brief Default constructor for IComponent.
     */
    IComponentRef() = default;

    /**
     * @brief Virtual destructor for IComponent.
     *
     * Ensures that derived components are properly destroyed.
     */
    virtual ~IComponentRef() = default;

};
} // namespace ECS::C
