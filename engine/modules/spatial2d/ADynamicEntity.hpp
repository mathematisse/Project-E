/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for ADynamicEntity declarations
*/

#pragma once

#include "AStaticEntity.hpp"
#include "Components.hpp"

namespace ECS::E {

// ENTITY REF
class ADynamicEntityRef : virtual public AStaticEntityRef, virtual public EntityWithVelocityRef, virtual public EntityWithAccelerationRef {
public:
    ADynamicEntityRef(
        const AStaticEntityRef &staticEnt, C::VelocityRef *velocity, C::AccelerationRef *acceleration
    );
    ~ADynamicEntityRef() override = default;
};

// ENTITY POOL
class ADynamicEntityPool : virtual public AStaticEntityPool, virtual public EntityWithVelocityPool, virtual public EntityWithAccelerationPool {
public:
    ADynamicEntityPool();
    ~ADynamicEntityPool() override = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::ADynamicEntityRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;
};
}
