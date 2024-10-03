/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Components/PureComponentPools.hpp"

#include "Systems/AMonoSystem.hpp"
#include "Systems/ADualSystem.hpp"
#include "Demo/ExamplePoint.hpp"


namespace ECS
{
    class MoveUpSystem : public Systems::AMonoSystem<Components::EntityStatusPool, Components::PositionPool, Components::VelocityPool>
    {
        public:
            explicit MoveUpSystem();
            ~MoveUpSystem() override = default;

            MoveUpSystem(const MoveUpSystem &other) = default;
            MoveUpSystem(MoveUpSystem &&other) = default;
            MoveUpSystem &operator=(const MoveUpSystem &other) = default;
            MoveUpSystem &operator=(MoveUpSystem &&other) = default;
        protected:
            void _innerOperate(typename Components::EntityStatusPool::Types& cstatus, typename Components::PositionPool::Types& cposition, typename Components::VelocityPool::Types& cvelocity) override;
    };

    class InitSystem : public Systems::AMonoSystem<Components::EntityStatusPool, Components::VelocityPool>
    {
        public:
            explicit InitSystem(float velocityMin = 0.5, float velocityMax = 1.5);
            ~InitSystem() override = default;

            InitSystem(const InitSystem &other) = default;
            InitSystem(InitSystem &&other) = default;
            InitSystem &operator=(const InitSystem &other) = default;
            InitSystem &operator=(InitSystem &&other) = default;

        protected:
            void _innerOperate(typename Components::EntityStatusPool::Types& cstatus, typename Components::VelocityPool::Types& cvelocity) override;
            float _velocityMin;
            float _velocityMax;
    };

    class PrintSystem : public Systems::AMonoSystem<Components::EntityStatusPool, Components::PositionPool>
    {
        public:
            explicit PrintSystem();
            ~PrintSystem() override = default;

            PrintSystem(const PrintSystem &other) = default;
            PrintSystem(PrintSystem &&other) = default;
            PrintSystem &operator=(const PrintSystem &other) = default;
            PrintSystem &operator=(PrintSystem &&other) = default;
        protected:
            void _innerOperate(typename Components::EntityStatusPool::Types& cstatus, typename Components::PositionPool::Types& cposition) override;
    };

    class DualExampleSystem : public Systems::ADualSystem<std::tuple<Components::EntityStatusPool, Components::ChunkPosPool>, std::tuple<Components::EntityStatusPool, Components::ChunkPosPool>>
    {
        public:
            explicit DualExampleSystem();
            ~DualExampleSystem() override = default;

            DualExampleSystem(const DualExampleSystem &other) = default;
            DualExampleSystem(DualExampleSystem &&other) = default;
            DualExampleSystem &operator=(const DualExampleSystem &other) = default;
            DualExampleSystem &operator=(DualExampleSystem &&other) = default;
        protected:
            void _innerOperate(
                Components::EntityStatusPool::Types& cStatusA,
                Components::ChunkPosPool::Types& cChunkPosA,
                Components::EntityStatusPool::Types& cStatusB,
                Components::ChunkPosPool::Types& cChunkPosB) override;
    };
}
