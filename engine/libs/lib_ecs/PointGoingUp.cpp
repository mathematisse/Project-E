/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "PointGoingUp.hpp"
#include "Components/PureComponentPools.hpp"
#include <iostream>
#include <iomanip>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

namespace ECS
{
    template <>
    const std::string Components::AComponentPool<float, float>::componentName = "position";

    // ENTITY POOL

    PointGoingUpEntityPool::PointGoingUpEntityPool() : AEntityPool("pointGoingUp", PointGoingUpEntityPoolChunkSize)
    {
    }

    PointGoingUpEntityPool::~PointGoingUpEntityPool() = default;

    std::unique_ptr<Entities::IEntityRef> PointGoingUpEntityPool::getEntity(Chunks::ChunkPos cPos)
    {
        return std::make_unique<ECS::PointGoingUpEntity>(
            static_cast<Components::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
            static_cast<Components::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
            static_cast<Components::PositionRef *>(_positionPool.getComponentRef(cPos)));
    }

    std::vector<Components::IComponentPool *> PointGoingUpEntityPool::getComponentPools()
    {
        return {&_entityStatusPool, &_chunkPosPool, &_positionPool};
    }

    // ENTITY

    PointGoingUpEntity::PointGoingUpEntity(Components::EntityStatusRef *status, Components::ChunkPosRef *cPos, Components::PositionRef *position)
        : AEntityRef(status, cPos), _position(position)
    {
    }

    PointGoingUpEntity::~PointGoingUpEntity() {
        delete _position;
    }

    std::tuple<float, float> PointGoingUpEntity::getPosition() const
    {
        return std::make_tuple(*_position->get<0>(), *_position->get<1>());
    }

    // SYSTEM

    MoveUpSystem::MoveUpSystem(float velocity)
        : ASystem(false), _velocity(velocity)
    {
    }


    void MoveUpSystem::innerOperate(Components::EntityStatusPool::Types& cstatus, Components::PositionPool::Types& cposition)
    {
        std::cout << RESET;
        auto [status] = cstatus;
        if ( status != Components::EntityStatusEnum::ENT_ALIVE) {
            std::cout << RED << "{x.x, x.x}" << RESET;
            return;
        }
        auto [x, y] = cposition;
        x += _velocity;
        y += _velocity;
        std::cout << GREEN << "{" << std::fixed << std::setprecision(1) << x << ", " << std::fixed << std::setprecision(1) << y << "}" << RESET;
    }

}