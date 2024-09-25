/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IComponent
*/

#pragma once


    namespace ECS::Components
    {
        class IComponent {
        public:
            IComponent() = default;
            virtual ~IComponent() = default;
            IComponent(const IComponent &other) = default;
            IComponent(IComponent &&other) = delete;
            IComponent &operator=(const IComponent &other) = delete;
            IComponent &operator=(IComponent &&other) = delete;
        };
    }

