/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "PointGoingUp.hpp"
#include <cstdint>

namespace ECS
{

    // COMPONENT POOL

    AltitudeComponentPool::AltitudeComponentPool() : AComponentPool<uint64_t>("altitude")
    {
    }

    // ENTITY POOL

    PointGoingUpEntityPool::PointGoingUpEntityPool() : AEntityPool("pointGoingUp", PointGoingUpEntityPoolChunkSize)
    {
    }

    PointGoingUpEntityPool::~PointGoingUpEntityPool()
    {
    }

    Entities::IEntity* PointGoingUpEntityPool::operator[](Chunks::ChunkPos cPos)
    {
        return new ECS::PointGoingUpEntity(
            dynamic_cast<Components::Component<uint8_t> *>(_entityStatusPool.getChunkPools()[cPos.chunkIndex]->getChunks()[cPos.elemIndex].get()),
            dynamic_cast<Components::Component2<uint64_t> *>(_chunkPosPool.getChunkPools()[cPos.chunkIndex]->getChunks()[cPos.elemIndex].get()),
            dynamic_cast<Components::Component<uint64_t> *>(_altitudePool.getChunkPools()[cPos.chunkIndex]->getChunks()[cPos.elemIndex].get()));
    }

    std::vector<Components::IComponentPool *> PointGoingUpEntityPool::getComponentPools()
    {
        return {&_entityStatusPool, &_chunkPosPool, &_altitudePool};
    }

    // ENTITY

    PointGoingUpEntity::PointGoingUpEntity(Components::Component<uint8_t> *status, Components::Component2<uint64_t> *cPos, Components::Component<uint64_t> *altitude)
        : AEntity(status, cPos), _altitude(altitude)
    {
    }

    PointGoingUpEntity::~PointGoingUpEntity()
    {
    }

    uint64_t PointGoingUpEntity::getAltitude() const
    {
        return *_altitude->GetX();
    }

    // SYSTEM

    MoveUpSystem::MoveUpSystem(unsigned int velocity)
        : ASystem({"altitude"}, false), _velocity(velocity)
    {
    }

    void MoveUpSystem::run()
    {
        // iterate over list of arrays of component pools (this case, arrays of one component pool)
        for (auto &componentPools : _componentPools) {
            // iterate over the component pool
            for (auto &chunkPool : dynamic_cast<AltitudeComponentPool *>(componentPools[0])->getChunkPools()) {
                // iterate over the chunk pool
                for (auto &chunk : chunkPool->getChunks()) {
                    // iterate over the c like array
                    uint64_t *array = chunk->GetData();
                    for (size_t i = 0; i < chunk->ElemCount(); i++) {
                        array[i] += _velocity;
                    }
                }
            }
        }
    }
}