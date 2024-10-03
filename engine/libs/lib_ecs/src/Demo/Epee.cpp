/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for Epee entity declarations
*/

#include "Epee.hpp"

// ENTITY
namespace ECS::Entities
{
    
    // ENTITY REF
    EpeeRef::EpeeRef(
        Components::EntityStatusRef *status,
        Components::ChunkPosRef *cPos,
        Components::DamageRef *Damage,
        Components::StatRef *Stat,
        Components::UpgradableRef *Upgradable
    ) : AEntityRef(status, cPos),
        _damage(Damage),
        _stat(Stat),
        _upgradable(Upgradable)
    {
    }

    EpeeRef::~EpeeRef()
    {
        delete _damage;
        delete _stat;
        delete _upgradable;
    }

    
    [[nodiscard]] Components::DamageRef *Entities::EpeeRef::getDamage() const
    {
        return _damage;
    }

    void Entities::EpeeRef::setDamage(const Components::DamageRef &damage)
    {
        *_damage = damage;
    }

    [[nodiscard]] Components::StatRef *Entities::EpeeRef::getStat() const
    {
        return _stat;
    }

    void Entities::EpeeRef::setStat(const Components::StatRef &stat)
    {
        *_stat = stat;
    }

    [[nodiscard]] Components::UpgradableRef *Entities::EpeeRef::getUpgradable() const
    {
        return _upgradable;
    }

    void Entities::EpeeRef::setUpgradable(const Components::UpgradableRef &upgradable)
    {
        *_upgradable = upgradable;
    }


    
    // ENTITY POOL
    EpeePool::EpeePool()
        : AEntityPool("Epee", EpeeChunkSize)
    {
    }

    std::unique_ptr<Entities::IEntityRef> EpeePool::getEntity(Chunks::ChunkPos cPos)
    {
        return getRawEntity(cPos);
    }

    std::unique_ptr<Entities::EpeeRef> EpeePool::getRawEntity(Chunks::ChunkPos cPos)
    {
        auto ptr = std::make_unique<Entities::EpeeRef>(
            static_cast<Components::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
            static_cast<Components::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
            static_cast<Components::DamageRef *>(_damagePool.getComponentRef(cPos)),
            static_cast<Components::StatRef *>(_statPool.getComponentRef(cPos)),
            static_cast<Components::UpgradableRef *>(_upgradablePool.getComponentRef(cPos))
        );
        return ptr;
    }

    std::vector<Components::IComponentPool *> EpeePool::getComponentPools()
    {
        return {
            &_entityStatusPool,
            &_chunkPosPool,
            &_damagePool,
            &_statPool,
            &_upgradablePool
        };
    }
}
