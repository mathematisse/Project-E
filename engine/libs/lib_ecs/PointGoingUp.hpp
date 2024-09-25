/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Entities/AEntityPool.hpp"
#include "Entities/AEntity.hpp"

#include "Systems/ASystem.hpp"


namespace ECS
{

    const size_t PointGoingUpEntityPoolChunkSize = 256;

    class AltitudeComponentPool : public Components::AComponentPool<uint64_t>
    {
        public:
            AltitudeComponentPool();
            ~AltitudeComponentPool() override = default;

            AltitudeComponentPool(const AltitudeComponentPool &other) = delete;
            AltitudeComponentPool(AltitudeComponentPool &&other) = delete;
            AltitudeComponentPool &operator=(const AltitudeComponentPool &other) = delete;
            AltitudeComponentPool &operator=(AltitudeComponentPool &&other) = delete;
    };


    class PointGoingUpEntityPool : public Entities::AEntityPool
    {
        public:
            PointGoingUpEntityPool();
            ~PointGoingUpEntityPool() override;

            PointGoingUpEntityPool(const PointGoingUpEntityPool &other) = delete;
            PointGoingUpEntityPool(PointGoingUpEntityPool &&other) = delete;
            PointGoingUpEntityPool &operator=(const PointGoingUpEntityPool &other) = delete;
            PointGoingUpEntityPool &operator=(PointGoingUpEntityPool &&other) = delete;

            Entities::IEntity* operator[](Chunks::ChunkPos cPos) override;
            std::vector<Components::IComponentPool *> getComponentPools() override;
        protected:
            AltitudeComponentPool _altitudePool;
    };

    class PointGoingUpEntity : public Entities::AEntity
    {
        public:
            PointGoingUpEntity(Components::Component<uint8_t> *status, Components::Component2<uint64_t> *cPos, Components::Component<uint64_t> *altitude);
            ~PointGoingUpEntity() override;

            PointGoingUpEntity(const PointGoingUpEntity &other) = delete;
            PointGoingUpEntity(PointGoingUpEntity &&other) = delete;
            PointGoingUpEntity &operator=(const PointGoingUpEntity &other) = delete;
            PointGoingUpEntity &operator=(PointGoingUpEntity &&other) = delete;

            [[nodiscard]] uint64_t getAltitude() const;
        protected:
            const Components::Component<uint64_t> *_altitude;
    };

    class MoveUpSystem : public Systems::ASystem
    {
        public:
            explicit MoveUpSystem(unsigned int velocity = 1);
            ~MoveUpSystem() override = default;

            MoveUpSystem(const MoveUpSystem &other) = delete;
            MoveUpSystem(MoveUpSystem &&other) = delete;
            MoveUpSystem &operator=(const MoveUpSystem &other) = delete;
            MoveUpSystem &operator=(MoveUpSystem &&other) = delete;

            void run() override;
        protected:
            unsigned int _velocity;
    };
}
