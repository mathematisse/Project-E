#pragma once

#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "modules/dummy/Components.hpp"

namespace ECS::S {

class RandomizeNumberSystem : public S::AMonoSystem<C::NumberPool> {
public:
    explicit RandomizeNumberSystem();
    ~RandomizeNumberSystem() override = default;

    RandomizeNumberSystem(const RandomizeNumberSystem &other) = default;
    RandomizeNumberSystem(RandomizeNumberSystem &&other) = default;
    RandomizeNumberSystem &operator=(const RandomizeNumberSystem &other) = default;
    RandomizeNumberSystem &operator=(RandomizeNumberSystem &&other) = default;

protected:
    void _innerOperate(typename C::NumberPool::Types &cnumber) override;
};

//////////////////////////////////////////////////////////////////////////////////

class ShowNumberSystem : public S::AMonoSystem<C::NumberPool> {
public:
    explicit ShowNumberSystem();
    ~ShowNumberSystem() override = default;

    ShowNumberSystem(const ShowNumberSystem &other) = default;
    ShowNumberSystem(ShowNumberSystem &&other) = default;
    ShowNumberSystem &operator=(const ShowNumberSystem &other) = default;
    ShowNumberSystem &operator=(ShowNumberSystem &&other) = default;

protected:
    void _innerOperate(typename C::NumberPool::Types &cnumber) override;
};

}