/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for Epee entity declarations
*/

#include "Epee.hpp"

// ENTITY
namespace ECS {
namespace E {

  // ENTITY REF
  EpeeRef::EpeeRef(C::EntityStatusRef *status,
    C::ChunkPosRef *cPos,
    C::DamageRef *Damage,
    C::StatRef *Stat,
    C::UpgradableRef *Upgradable)
    : AEntityRef(status, cPos), _damage(Damage), _stat(Stat), _upgradable(Upgradable)
  {}

  EpeeRef::~EpeeRef()
  {
    delete _damage;
    delete _stat;
    delete _upgradable;
  }


  [[nodiscard]] C::DamageRef *E::EpeeRef::getDamage() const { return _damage; }

  void E::EpeeRef::setDamage(const C::DamageRef &damage) { *_damage = damage; }

  [[nodiscard]] C::StatRef *E::EpeeRef::getStat() const { return _stat; }

  void E::EpeeRef::setStat(const C::StatRef &stat) { *_stat = stat; }

  [[nodiscard]] C::UpgradableRef *E::EpeeRef::getUpgradable() const { return _upgradable; }

  void E::EpeeRef::setUpgradable(const C::UpgradableRef &upgradable) { *_upgradable = upgradable; }


  // ENTITY POOL
  EpeePool::EpeePool() : AEntityPool("Epee", EpeeChunkSize) {}

  std::unique_ptr<E::IEntityRef> EpeePool::getEntity(Chunks::chunkPos_t cPos) { return getRawEntity(cPos); }

  std::unique_ptr<E::EpeeRef> EpeePool::getRawEntity(Chunks::chunkPos_t cPos)
  {
    auto ptr = std::make_unique<E::EpeeRef>(dynamic_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
      dynamic_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
      dynamic_cast<C::DamageRef *>(_damagePool.getComponentRef(cPos)),
      dynamic_cast<C::StatRef *>(_statPool.getComponentRef(cPos)),
      dynamic_cast<C::UpgradableRef *>(_upgradablePool.getComponentRef(cPos)));
    return ptr;
  }

  std::vector<C::IComponentPool *> EpeePool::getComponentPools()
  {
    return { &_entityStatusPool, &_chunkPosPool, &_damagePool, &_statPool, &_upgradablePool };
  }
}// namespace E
}// namespace ECS
