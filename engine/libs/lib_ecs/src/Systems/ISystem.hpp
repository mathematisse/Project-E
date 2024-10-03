/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base ISystem
*/

#pragma once

#include "Entities/IEntityPool.hpp"


namespace ECS::Systems {
class ISystem
{
public:
  ISystem() = default;
  virtual ~ISystem() = default;
  ISystem(const ISystem &other) = default;
  ISystem(ISystem &&other) = default;
  ISystem &operator=(const ISystem &other) = default;
  ISystem &operator=(ISystem &&other) = default;
  virtual bool tryAddEntityPool(Entities::IEntityPool *entityPool) = 0;
  virtual void run() = 0;
};
}// namespace ECS::Systems
