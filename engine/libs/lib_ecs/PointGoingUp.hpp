/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Chunks/ChunkPos.hpp"
#include "Components/PureComponentPools.hpp"
#include "Entities/AEntityPool.hpp"
#include "Entities/AEntityRef.hpp"

#include "Systems/ASystem.hpp"


namespace ECS
{
    namespace Components
    {
        DECLARE_COMPONENT(Position, float, float);
    }

    const size_t PointGoingUpEntityPoolChunkSize = 5;

    class PointGoingUpEntityPool : public Entities::AEntityPool
    {
        public:
            PointGoingUpEntityPool();
            ~PointGoingUpEntityPool() override;

            PointGoingUpEntityPool(const PointGoingUpEntityPool &other) = default;
            PointGoingUpEntityPool(PointGoingUpEntityPool &&other) = default;
            PointGoingUpEntityPool &operator=(const PointGoingUpEntityPool &other) = default;
            PointGoingUpEntityPool &operator=(PointGoingUpEntityPool &&other) = default;

            std::unique_ptr<Entities::IEntityRef> getEntity(Chunks::ChunkPos cPos) override;
            std::vector<Components::IComponentPool *> getComponentPools() override;
        protected:
            Components::PositionPool _positionPool;
    };

    class PointGoingUpEntity : public Entities::AEntityRef
    {
        public:
            PointGoingUpEntity(Components::EntityStatusRef *status, Components::ChunkPosRef *cPos, Components::PositionRef *position);
            ~PointGoingUpEntity() override;

            PointGoingUpEntity(const PointGoingUpEntity &other) = default;
            PointGoingUpEntity(PointGoingUpEntity &&other) = default;
            PointGoingUpEntity &operator=(const PointGoingUpEntity &other) = default;
            PointGoingUpEntity &operator=(PointGoingUpEntity &&other) = default;

            [[nodiscard]] std::tuple<float, float> getPosition() const;
        protected:
            const Components::PositionRef *_position;
    };

    class MoveUpSystem : public Systems::ASystem<Components::EntityStatusPool, Components::PositionPool>
    {
        public:
            explicit MoveUpSystem(float velocity = 0.5);
            ~MoveUpSystem() override = default;

            MoveUpSystem(const MoveUpSystem &other) = default;
            MoveUpSystem(MoveUpSystem &&other) = default;
            MoveUpSystem &operator=(const MoveUpSystem &other) = default;
            MoveUpSystem &operator=(MoveUpSystem &&other) = default;

            void innerOperate(typename Components::EntityStatusPool::Types& cstatus, typename Components::PositionPool::Types& cposition) override;
        protected:
            float _velocity;
    };
}
