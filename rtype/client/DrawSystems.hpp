/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "DecorEntities.hpp"
#include "GameEntities.hpp"
#include "lib_ecs/Systems/AMonoSystem.hpp"
#include "raylib.h"

namespace ECS::S {



class ShowInfoSystem : public S::AMonoSystem<C::TypePool, C::HealthPool> {
public:
    explicit ShowInfoSystem(Camera2D &camera);
    ~ShowInfoSystem() override = default;


    Camera2D &camera;
    bool one_time = false;

protected:
    void _innerOperate(C::TypePool::Types &ctype, C::HealthPool::Types &chealth) override;
};


} // namespace ECS::S
