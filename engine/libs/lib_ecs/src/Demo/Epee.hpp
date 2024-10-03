/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for Epee entity declarations
*/

#pragma once

#include "Components/PureComponentPools.hpp"
#include "Entities/AEntityRef.hpp"
#include "Entities/AEntityPool.hpp"

namespace ECS::Components
{
    DECLARE_COMPONENT(Damage, float, float, uint8_t);
    DECLARE_COMPONENT(Stat, long, long, long, long, long, long, uint64_t);
    DECLARE_COMPONENT(Upgradable, bool);
}

namespace ECS::Entities
{
    const size_t EpeeChunkSize = 32;

    // ENTITY REF
    class EpeeRef : public AEntityRef
    {
    public:
        EpeeRef(
            Components::EntityStatusRef *status,
            Components::ChunkPosRef *cPos,
            Components::DamageRef *damage,
            Components::StatRef *stat,
            Components::UpgradableRef *upgradable
        );
        ~EpeeRef() override;

        // Default rule of five
        EpeeRef(const EpeeRef &other) = default;
        EpeeRef &operator=(const EpeeRef &other) = default;
        EpeeRef(EpeeRef &&other) = default;
        EpeeRef &operator=(EpeeRef &&other) = default;

        // Accessors
        
        [[nodiscard]] Components::DamageRef *getDamage() const;
        void setDamage(const Components::DamageRef &damage);

        [[nodiscard]] Components::StatRef *getStat() const;
        void setStat(const Components::StatRef &stat);

        [[nodiscard]] Components::UpgradableRef *getUpgradable() const;
        void setUpgradable(const Components::UpgradableRef &upgradable);


    protected:
        Components::DamageRef *_damage;
        Components::StatRef *_stat;
        Components::UpgradableRef *_upgradable;
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

        std::unique_ptr<Entities::IEntityRef> getEntity(Chunks::ChunkPos cPos) override;
        std::unique_ptr<Entities::EpeeRef> getRawEntity(Chunks::ChunkPos cPos);
        std::vector<Components::IComponentPool *> getComponentPools() override;
    protected:
        Components::DamagePool _damagePool;
        Components::StatPool _statPool;
        Components::UpgradablePool _upgradablePool;
    };
}

