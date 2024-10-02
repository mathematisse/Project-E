/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "PointGoingUp.hpp"
#include "Components/PureComponentPools.hpp"
#include <cstdint>
#include <iostream>

namespace ECS
{

    // COMPONENT POOL

    AltitudeComponentPool::AltitudeComponentPool() : AComponentPool<Chunks::chunk_pos_t>("altitude")
    {
    }

    // ENTITY POOL

    PointGoingUpEntityPool::PointGoingUpEntityPool() : AEntityPool("pointGoingUp", PointGoingUpEntityPoolChunkSize)
    {
    }

    PointGoingUpEntityPool::~PointGoingUpEntityPool() = default;

    std::unique_ptr<Entities::IEntity> PointGoingUpEntityPool::getEntity(Chunks::ChunkPos cPos)
    {
        return std::make_unique<ECS::PointGoingUpEntity>(
            dynamic_cast<Components::ComponentRef<Components::entity_status_t> *>(_entityStatusPool.getComponentRef(cPos)),
            dynamic_cast<Components::ComponentRef2<Chunks::chunk_pos_t> *>(_chunkPosPool.getComponentRef(cPos)),
            dynamic_cast<Components::ComponentRef<Chunks::chunk_pos_t> *>(_altitudePool.getComponentRef(cPos)));
    }

    std::vector<Components::IComponentPool *> PointGoingUpEntityPool::getComponentPools()
    {
        return {&_entityStatusPool, &_chunkPosPool, &_altitudePool};
    }

    // ENTITY

    PointGoingUpEntity::PointGoingUpEntity(Components::ComponentRef<Components::entity_status_t> *status, Components::ComponentRef2<Chunks::chunk_pos_t> *cPos, Components::ComponentRef<Chunks::chunk_pos_t> *altitude)
        : AEntity(status, cPos), _altitude(altitude)
    {
    }

    PointGoingUpEntity::~PointGoingUpEntity() {
        delete _altitude;
    }

    Chunks::chunk_pos_t PointGoingUpEntity::getAltitude() const
    {
        return *_altitude->getX();
    }

    // SYSTEM

    MoveUpSystem::MoveUpSystem(unsigned int velocity)
        : ASystem({"altitude"}, false), _velocity(velocity)
    {
    }

    void MoveUpSystem::run()
    {
        std::cout << "Running MoveUpSystem\n";
        // iterate over list of arrays of component pools (this case, arrays of one component pool)
        for (auto &componentPools : _componentPools) {
            // iterate over the component pool
            for (auto &chunkPool : dynamic_cast<AltitudeComponentPool *>(componentPools[0])->getChunkPools()) {
                // iterate over the chunk pool
                for (auto &chunk : chunkPool->getChunks()) {
                    // iterate over the c like array
                    for (size_t i = 0; i < chunk->elemCount(); i++) {
                        *(chunk->getElem(i)) += _velocity;
                    }
                }
            }
        }
    }
}