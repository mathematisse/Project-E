/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for Epee entity declarations
*/

#pragma once

#include "Components/PureComponentPools.hpp"
#include "Entities/AEntityPool.hpp"
#include "Entities/AEntityRef.hpp"

namespace ECS::C {
DECLARE_COMPONENT(Damage, float, float, uint8_t);
DECLARE_COMPONENT(Stat, long, long, long, long, long, long, uint64_t);
DECLARE_COMPONENT(Upgradable, bool);
}// namespace ECS::C

namespace ECS::E {
const size_t EpeeChunkSize = 32;

// ENTITY REF
class EpeeRef : public AEntityRef
{
public:
  EpeeRef(C::EntityStatusRef *status,
    C::ChunkPosRef *cPos,
    C::DamageRef *damage,
    C::StatRef *stat,
    C::UpgradableRef *upgradable);
  ~EpeeRef() override;

  // Default rule of five
  EpeeRef(const EpeeRef &other) = default;
  EpeeRef &operator=(const EpeeRef &other) = default;
  EpeeRef(EpeeRef &&other) = default;
  EpeeRef &operator=(EpeeRef &&other) = default;

  // Accessors

  [[nodiscard]] C::DamageRef *getDamage() const;
  void setDamage(const C::DamageRef &damage);

  [[nodiscard]] C::StatRef *getStat() const;
  void setStat(const C::StatRef &stat);

  [[nodiscard]] C::UpgradableRef *getUpgradable() const;
  void setUpgradable(const C::UpgradableRef &upgradable);


protected:
  C::DamageRef *_damage;
  C::StatRef *_stat;
  C::UpgradableRef *_upgradable;
};

// ENTITY POOL
class EpeePool : public AEntityPool
{
public:
  EpeePool();
  ~EpeePool() override = default;

  // default rule of five
  EpeePool(const EpeePool &other) = default;
  EpeePool &operator=(const EpeePool &other) = default;
  EpeePool(EpeePool &&other) = default;
  EpeePool &operator=(EpeePool &&other) = default;

  std::unique_ptr<E::IEntityRef> getEntity(Chunks::ChunkPos cPos) override;
  std::unique_ptr<E::EpeeRef> getRawEntity(Chunks::ChunkPos cPos);
  std::vector<C::IComponentPool *> getComponentPools() override;

protected:
  C::DamagePool _damagePool;
  C::StatPool _statPool;
  C::UpgradablePool _upgradablePool;
};
}// namespace ECS::E
