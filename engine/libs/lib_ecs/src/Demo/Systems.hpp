/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Components/PureComponentPools.hpp"

#include "Systems/ASystem.hpp"
#include "Demo/ExamplePoint.hpp"


namespace ECS
{
    class MoveUpSystem : public Systems::ASystem<Components::EntityStatusPool, Components::PositionPool, Components::VelocityPool>
    {
        public:
            explicit MoveUpSystem();
            ~MoveUpSystem() override = default;

            MoveUpSystem(const MoveUpSystem &other) = default;
            MoveUpSystem(MoveUpSystem &&other) = default;
            MoveUpSystem &operator=(const MoveUpSystem &other) = default;
            MoveUpSystem &operator=(MoveUpSystem &&other) = default;

            void innerOperate(typename Components::EntityStatusPool::Types& cstatus, typename Components::PositionPool::Types& cposition, typename Components::VelocityPool::Types& cvelocity) override;
    };

    class InitSystem : public Systems::ASystem<Components::EntityStatusPool, Components::VelocityPool>
    {
        public:
            explicit InitSystem(float velocityMin = 0.5, float velocityMax = 1.5);
            ~InitSystem() override = default;

            InitSystem(const InitSystem &other) = default;
            InitSystem(InitSystem &&other) = default;
            InitSystem &operator=(const InitSystem &other) = default;
            InitSystem &operator=(InitSystem &&other) = default;

            void innerOperate(typename Components::EntityStatusPool::Types& cstatus, typename Components::VelocityPool::Types& cvelocity) override;
        protected:
            float _velocityMin;
            float _velocityMax;
    };

    class PrintSystem : public Systems::ASystem<Components::EntityStatusPool, Components::PositionPool>
    {
        public:
            explicit PrintSystem();
            ~PrintSystem() override = default;

            PrintSystem(const PrintSystem &other) = default;
            PrintSystem(PrintSystem &&other) = default;
            PrintSystem &operator=(const PrintSystem &other) = default;
            PrintSystem &operator=(PrintSystem &&other) = default;

            void innerOperate(typename Components::EntityStatusPool::Types& cstatus, typename Components::PositionPool::Types& cposition) override;
    };
}
