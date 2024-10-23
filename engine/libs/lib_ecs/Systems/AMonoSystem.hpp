/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ASystem
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/IEntityPool.hpp"
#include "lib_ecs/Systems/ASystem.hpp"
#include "lib_ecs/Systems/Query.hpp"
#include <iostream>
// # include <chrono>

namespace ECS::S {
template<typename... Ts>
class AMonoSystem : public ASystem {
public:
    explicit AMonoSystem(bool isParallel):
        ASystem(isParallel)
    {
    }
    ~AMonoSystem() override = default;


    bool tryAddEntityPool(E::IEntityPool *entityPool) override
    {
        return _query.tryAddEntityPool(entityPool);
    }

    void run() override
    {
        _query.map(
            [this](auto &...componentPools) {
                _innerOperate(componentPools...);
            },
            _isParallel,
            [this](auto &...componentPools) {
                _startCMapCallBack(componentPools...);
            },
            [this](auto &...componentPools) {
                _endCMapCallBack(componentPools...);
            },
            [this](auto *...componentPools) {
                _startCPMapCallBack(componentPools...);
            },
            [this](auto *...componentPools) {
                _endCPMapCallBack(componentPools...);
            }
        );
    }

protected:
    Query<Ts...> _query;

    virtual inline void _innerOperate(typename Ts::Types &...componentPools) = 0;
    virtual void _startCMapCallBack(typename Ts::VTypes &.../*unused*/) { }
    virtual void _endCMapCallBack(typename Ts::VTypes &.../*unused*/) { }
    virtual void _startCPMapCallBack(Ts *.../*unused*/) { }
    virtual void _endCPMapCallBack(Ts *.../*unused*/) { }
};

template<typename... Ts>
class AStatusMonoSystem : public AMonoSystem<C::EntityStatusPool, Ts...> {
public:
    explicit AStatusMonoSystem(bool isParallel = false, C::EntityStatusEnum status = C::ENT_ALIVE):
        AMonoSystem<C::EntityStatusPool, Ts...>(isParallel),
        _status(status)
    {
    }
    ~AStatusMonoSystem() override = default;


protected:
    C::EntityStatusEnum _status;
    void _innerOperate(C::EntityStatusPool::Types &cStatus, typename Ts::Types &...componentPools)
        override
    {
        auto [status] = cStatus;
        if (status == _status) {
            _statusOperate(componentPools...);
        }
    };

    virtual void _statusOperate(typename Ts::Types &...componentPools) = 0;
};

} // namespace ECS::S
