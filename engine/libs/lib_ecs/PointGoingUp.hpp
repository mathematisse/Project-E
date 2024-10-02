/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Components/PureComponentPools.hpp"
#include "Entities/AEntityPool.hpp"
#include "Entities/AEntity.hpp"

#include "Systems/ASystem.hpp"


namespace ECS
{

    const size_t PointGoingUpEntityPoolChunkSize = 256;

    class AltitudeComponentPool : public Components::AComponentPool<Chunks::chunk_pos_t>
    {
        public:
            AltitudeComponentPool();
            ~AltitudeComponentPool() override = default;

            AltitudeComponentPool(const AltitudeComponentPool &other) = default;
            AltitudeComponentPool(AltitudeComponentPool &&other) = default;
            AltitudeComponentPool &operator=(const AltitudeComponentPool &other) = default;
            AltitudeComponentPool &operator=(AltitudeComponentPool &&other) = default;
    };


    class PointGoingUpEntityPool : public Entities::AEntityPool
    {
        public:
            PointGoingUpEntityPool();
            ~PointGoingUpEntityPool() override;

            PointGoingUpEntityPool(const PointGoingUpEntityPool &other) = default;
            PointGoingUpEntityPool(PointGoingUpEntityPool &&other) = default;
            PointGoingUpEntityPool &operator=(const PointGoingUpEntityPool &other) = default;
            PointGoingUpEntityPool &operator=(PointGoingUpEntityPool &&other) = default;

            std::unique_ptr<Entities::IEntity> getEntity(Chunks::ChunkPos cPos) override;
            std::vector<Components::IComponentPool *> getComponentPools() override;
        protected:
            AltitudeComponentPool _altitudePool;
    };

    class PointGoingUpEntity : public Entities::AEntity
    {
        public:
            PointGoingUpEntity(Components::ComponentRef<Components::entity_status_t> *status, Components::ComponentRef2<Chunks::chunk_pos_t> *cPos, Components::ComponentRef<Chunks::chunk_pos_t> *altitude);
            ~PointGoingUpEntity() override;

            PointGoingUpEntity(const PointGoingUpEntity &other) = default;
            PointGoingUpEntity(PointGoingUpEntity &&other) = default;
            PointGoingUpEntity &operator=(const PointGoingUpEntity &other) = default;
            PointGoingUpEntity &operator=(PointGoingUpEntity &&other) = default;

            [[nodiscard]] Chunks::chunk_pos_t getAltitude() const;
        protected:
            const Components::ComponentRef<Chunks::chunk_pos_t> *_altitude;
    };

    class MoveUpSystem : public Systems::ASystem
    {
        public:
            explicit MoveUpSystem(unsigned int velocity = 1);
            ~MoveUpSystem() override = default;

            MoveUpSystem(const MoveUpSystem &other) = default;
            MoveUpSystem(MoveUpSystem &&other) = default;
            MoveUpSystem &operator=(const MoveUpSystem &other) = default;
            MoveUpSystem &operator=(MoveUpSystem &&other) = default;

            void run() override;
        protected:
            unsigned int _velocity;
    };
}
