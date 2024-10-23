/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for AStaticEntity declarations
*/

#pragma once

#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"
#include "Components.hpp"

namespace ECS::E {

// ENTITY REF
class AStaticEntityRef : virtual public AEntityRef,
                         virtual public EntityWithPositionRef,
                         virtual public EntityWithRotationRef {
public:
    AStaticEntityRef(const AEntityRef &ent, C::PositionRef *position, C::RotationRef *rotation);
    ~AStaticEntityRef() override = default;
};

// ENTITY POOL
class AStaticEntityPool : virtual public AEntityPool,
                          virtual public EntityWithPositionPool,
                          virtual public EntityWithRotationPool {
public:
    AStaticEntityPool();
    ~AStaticEntityPool() override = default;

    std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
    std::unique_ptr<E::AStaticEntityRef> getRawEntity(Chunks::chunkPos_t cPos);
    std::vector<C::IComponentPool *> getComponentPools() override;
};
}
