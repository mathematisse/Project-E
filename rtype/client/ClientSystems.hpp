/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "DecorSquare.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "Square.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/ADualSystem.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "AssetsLoader.hpp"
#include "NetworkManager.hpp"
#include "raylib.h"

namespace ECS::S {

class MovePlayerSystem : public S::AStatusMonoSystem<C::VelocityPool, C::TypePool> {
public:
    explicit MovePlayerSystem();
    ~MovePlayerSystem() override = default;

    MovePlayerSystem(const MovePlayerSystem &other) = default;
    MovePlayerSystem(MovePlayerSystem &&other) = default;
    MovePlayerSystem &operator=(const MovePlayerSystem &other) = default;
    MovePlayerSystem &operator=(MovePlayerSystem &&other) = default;

protected:
    void
    _statusOperate(typename C::VelocityPool::Types &cvelocity, typename C::TypePool::Types &ctype) override;
};

class UpdateEnginePosition : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool, C::TypePool> {
public:
    explicit UpdateEnginePosition();
    ~UpdateEnginePosition() override = default;

    UpdateEnginePosition(const UpdateEnginePosition &other) = default;
    UpdateEnginePosition(UpdateEnginePosition &&other) = default;
    UpdateEnginePosition &operator=(const UpdateEnginePosition &other) = default;
    UpdateEnginePosition &operator=(UpdateEnginePosition &&other) = default;

    Vector2 playerPosition;
    char playerAlive;

protected:
    void _innerOperate(
        C::EntityStatusPool::Types &cstatus, C::PositionPool::Types &cposition, C::TypePool::Types &ctype
    ) override;
};

} // namespace ECS::S
