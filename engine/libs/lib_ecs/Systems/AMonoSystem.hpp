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

namespace ECS::S {
template<typename... Ts>
class AMonoSystem : public ASystem {
public:
    explicit AMonoSystem(bool isParallel):
        ASystem(isParallel)
    {
    }
    ~AMonoSystem() override = default;

    AMonoSystem(const AMonoSystem &other) = default;
    AMonoSystem(AMonoSystem &&other) = default;
    AMonoSystem &operator=(const AMonoSystem &other) = default;
    AMonoSystem &operator=(AMonoSystem &&other) = default;

    bool tryAddEntityPool(E::IEntityPool *entityPool) override { return _query.tryAddEntityPool(entityPool); }

    void run() override
    {
        std::cout << "Running system\n";
        _query.map(
            [this](auto &...componentPools) {
                _innerOperate(componentPools...);
            },
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
        std::cout << "System done\n";
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
    explicit AStatusMonoSystem(bool isParallel, C::EntityStatusEnum status):
        AMonoSystem<C::EntityStatusPool, Ts...>(isParallel),
        _status(status)
    {
    }
    ~AStatusMonoSystem() override = default;

    AStatusMonoSystem(const AStatusMonoSystem &other) = default;
    AStatusMonoSystem(AStatusMonoSystem &&other) = default;
    AStatusMonoSystem &operator=(const AStatusMonoSystem &other) = default;
    AStatusMonoSystem &operator=(AStatusMonoSystem &&other) = default;

protected:
    C::EntityStatusEnum _status;
    void _innerOperate(C::EntityStatusPool::Types &cStatus, typename Ts::Types &...componentPools) override
    {
        auto [status] = cStatus;
        if (status == _status) {
            _statusOperate(componentPools...);
        } else {
            std::cout << "Skipping operation for entity with status " << int(status) << "\n";
        }
    };

    virtual void _statusOperate(typename Ts::Types &...componentPools) = 0;
};

} // namespace ECS::S
