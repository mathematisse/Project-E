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

class SendDecorStateSystem : public S::AMonoSystem<
                                 C::EntityStatusPool, C::PositionPool, C::SizePool, C::TypePool,
                                 C::SpritePool, C::NetworkIDPool> {
public:
    explicit SendDecorStateSystem();
    ~SendDecorStateSystem() override = default;

    SendDecorStateSystem(const SendDecorStateSystem &other) = default;
    SendDecorStateSystem(SendDecorStateSystem &&other) = default;
    SendDecorStateSystem &operator=(const SendDecorStateSystem &other) = default;
    SendDecorStateSystem &operator=(SendDecorStateSystem &&other) = default;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cpos,
        typename C::SizePool::Types &csize, typename C::TypePool::Types &ctype,
        typename C::SpritePool::Types &csprite, typename C::NetworkIDPool::Types &cnet
    ) override;
};

class SendSquareStateSystem : public S::AMonoSystem<
                                  C::EntityStatusPool, C::PositionPool, C::VelocityPool,
                                  C::ColorPool, C::SizePool, C::TypePool, C::CanShootPool,
                                  C::SpritePool, C::HealthPool, C::TimerPool, C::NetworkIDPool> {
public:
    explicit SendSquareStateSystem();
    ~SendSquareStateSystem() override = default;

    SendSquareStateSystem(const SendSquareStateSystem &other) = default;
    SendSquareStateSystem(SendSquareStateSystem &&other) = default;
    SendSquareStateSystem &operator=(const SendSquareStateSystem &other) = default;
    SendSquareStateSystem &operator=(SendSquareStateSystem &&other) = default;

protected:
    void _innerOperate(
        typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cpos,
        typename C::VelocityPool::Types &cvel, typename C::ColorPool::Types &ccol,
        typename C::SizePool::Types &csize, typename C::TypePool::Types &ctype,
        typename C::CanShootPool::Types &cshoot, typename C::SpritePool::Types &csprite,
        typename C::HealthPool::Types &chealth, typename C::TimerPool::Types &ctimer,
        typename C::NetworkIDPool::Types &cnet
    ) override;
};

} // namespace ECS::S
