/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Demo lib ecs
*/

#include "Components/PureComponentPools.hpp"

#include "Demo/ExamplePoint.hpp"
#include "Systems/ADualSystem.hpp"
#include "Systems/AMonoSystem.hpp"


namespace ECS::S {
class MoveUpSystem : public S::AStatusMonoSystem<C::PositionPool, C::VelocityPool>
{
public:
  explicit MoveUpSystem();
  ~MoveUpSystem() override = default;

  MoveUpSystem(const MoveUpSystem &other) = default;
  MoveUpSystem(MoveUpSystem &&other) = default;
  MoveUpSystem &operator=(const MoveUpSystem &other) = default;
  MoveUpSystem &operator=(MoveUpSystem &&other) = default;

protected:
  void _statusOperate(typename C::PositionPool::Types &cposition, typename C::VelocityPool::Types &cvelocity) override;
};

class InitSystem : public S::AMonoSystem<C::EntityStatusPool, C::VelocityPool>
{
public:
  explicit InitSystem(float velocityMin = 0.5, float velocityMax = 1.5);
  ~InitSystem() override = default;

  InitSystem(const InitSystem &other) = default;
  InitSystem(InitSystem &&other) = default;
  InitSystem &operator=(const InitSystem &other) = default;
  InitSystem &operator=(InitSystem &&other) = default;

protected:
  void _innerOperate(typename C::EntityStatusPool::Types &cstatus, typename C::VelocityPool::Types &cvelocity) override;
  float _velocityMin;
  float _velocityMax;
};

class PrintSystem : public S::AMonoSystem<C::EntityStatusPool, C::PositionPool>
{
public:
  explicit PrintSystem();
  ~PrintSystem() override = default;

  PrintSystem(const PrintSystem &other) = default;
  PrintSystem(PrintSystem &&other) = default;
  PrintSystem &operator=(const PrintSystem &other) = default;
  PrintSystem &operator=(PrintSystem &&other) = default;

protected:
  void _innerOperate(typename C::EntityStatusPool::Types &cstatus, typename C::PositionPool::Types &cposition) override;
};

class DualExampleSystem
  : public S::ADualSystem<std::tuple<C::EntityStatusPool, C::ChunkPosPool>,
      std::tuple<C::EntityStatusPool, C::ChunkPosPool>>
{
public:
  explicit DualExampleSystem();
  ~DualExampleSystem() override = default;

  DualExampleSystem(const DualExampleSystem &other) = default;
  DualExampleSystem(DualExampleSystem &&other) = default;
  DualExampleSystem &operator=(const DualExampleSystem &other) = default;
  DualExampleSystem &operator=(DualExampleSystem &&other) = default;

protected:
  void _innerOperate(C::EntityStatusPool::Types &cStatusA,
    C::ChunkPosPool::Types &cChunkPosA,
    C::EntityStatusPool::Types &cStatusB,
    C::ChunkPosPool::Types &cChunkPosB) override;
};
}// namespace ECS::S
