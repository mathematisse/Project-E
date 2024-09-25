/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "Components/IComponentPool.hpp"
#include "Entities/IEntityPool.hpp"
#include "Systems/ISystem.hpp"
#include <vector>
#include <string>


    namespace ECS::Systems
    {

        class ASystem : public ISystem
        {
        public:
            ASystem(const std::vector<std::string> &matches, bool isParallel);
            ~ASystem() override;
            ASystem(const ASystem &other) = delete;
            ASystem(ASystem &&other) = delete;
            ASystem &operator=(const ASystem &other) = delete;
            ASystem &operator=(ASystem &&other) = delete;
            [[nodiscard]] const std::vector<std::string> &getMatches() const;
            [[nodiscard]] bool getIsParallel() const;
            bool tryAddEntityPool(Entities::IEntityPool *entityPool) override;
            void run() override = 0;
        protected:
            const std::vector<std::string> &_matches;
            const bool _isParallel;

            std::list<std::vector<Components::IComponentPool *>> _componentPools;
        };
    }
