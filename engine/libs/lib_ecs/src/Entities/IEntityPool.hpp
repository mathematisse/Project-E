/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base IEntityPool
*/

#pragma once

#include "Entities/IEntityRef.hpp"
#include "Components/IComponentPool.hpp"
#include <list>
#include <memory>
#include <vector>

#define EXPAND(x) x

#define COMPONENT_POOL_ELEMENT(component) \
    Components::component##Pool _component##Pool; \

#define COMPONENT_POOL_REST_ARGS(...) \
    EXPAND(COMPONENT_POOL_FIRST_ARG(__VA_ARGS__))

#define COMPONENT_POOL_REST_ARGS(...)

// COMPONENT_POOL macros
#define COMPONENT_POOL_FIRST_ARG(c, ...) COMPONENT_POOL_ELEMENT(c) COMPONENT_POOL_REST_ARGS(__VA_ARGS__)

// Constructor macros
#define CONSTRUCTOR_ELEMENT(component) \
    Components::component##Ref *component, \

#define CONSTRUCTOR_REST_ARGS(...) \
    EXPAND(CONSTRUCTOR_FIRST_ARG(__VA_ARGS__))

#define CONSTRUCTOR_REST_ARGS(...)

#define CONSTRUCTOR_FIRST_ARG(c, ...) CONSTRUCTOR_ELEMENT(c) CONSTRUCTOR_REST_ARGS(__VA_ARGS__)

// Accessor macros
#define ACCESSORS_ELEMENT(component) \
    [[nodiscard]] Components::component##Ref *get##component() const; \
    void set##component(const Components::component##Ref &component); \

#define ACCESSORS_FIRST_ARG(c, ...) ACCESSORS_ELEMENT(c) ACCESSORS_REST_ARGS(__VA_ARGS__)

#define ACCESSORS_REST_ARGS(...) \
    EXPAND(ACCESSORS_FIRST_ARG(__VA_ARGS__))

#define ACCESSORS_REST_ARGS(...)

// Component references
#define COMPONENT_REFS_ELEMENT(component) \
    Components::component##Ref *_##component; \

#define COMPONENT_REFS_FIRST_ARG(c, ...) COMPONENT_REFS_ELEMENT(c) COMPONENT_REFS_REST_ARGS(__VA_ARGS__)

#define COMPONENT_REFS_REST_ARGS(...) \
    EXPAND(COMPONENT_REFS_FIRST_ARG(__VA_ARGS__))

#define COMPONENT_REFS_REST_ARGS(...)

// Adjusted entity declaration
#define _DECLARE_ENTITY_HPP(name, ...) \
    class name##Ref : public AEntityRef \
    { \
    public: \
        name##Ref( \
            CONSTRUCTOR_REST_ARGS(__VA_ARGS__) \
            ); \
        ~name##Ref() override; \
        name##Ref(const name##Ref &other) = default; \
        name##Ref(name##Ref &&other) = default; \
        name##Ref &operator=(const name##Ref &other) = default; \
        name##Ref &operator=(name##Ref &&other) = default; \
        ACCESSORS_REST_ARGS(__VA_ARGS__) \
    protected: \
        COMPONENT_REFS_REST_ARGS(__VA_ARGS__) \
    }; \
    \
    class name##Pool : public AEntityPool \
    { \
    public: \
        name##Pool(); \
        std::unique_ptr<IEntityRef> getEntity(Chunks::ChunkPos cPos) override; \
        std::unique_ptr<Entities::name##Ref> getRawEntity(Chunks::ChunkPos cPos); \
        std::vector<Components::IComponentPool *> getComponentPools() override; \
    protected: \
        COMPONENT_POOL_REST_ARGS(__VA_ARGS__) \
    }; \


#define DECLARE_ENTITY_HPP(name, ...) \
    _DECLARE_ENTITY_HPP(name,  EntityStatus, ChunkPos, __VA_ARGS__)

#define GETRAWENTS_ELEMENT(component) \
    static_cast<Components::component##Pool *>(_component##Pool.getComponentPool())->getComponentRef(cPos), \

#define GETRAWENTS_FIRST_ARG(c, ...) GETRAWENTS_ELEMENT(c) GETRAWENTS_REST_ARGS(__VA_ARGS__)

#define GETRAWENTS_REST_ARGS(...) \
    EXPAND(GETRAWENTS_FIRST_ARG(__VA_ARGS__))

#define GETRAWENTS_REST_ARGS(...)

#define GETCOMPONENTSPOOLS_ELEMENT(component) \
    &_component##Pool, \

#define GETCOMPONENTSPOOLS_FIRST_ARG(c, ...) GETCOMPONENTSPOOLS_ELEMENT(c) GETCOMPONENTSPOOLS_REST_ARGS(__VA_ARGS__)

#define GETCOMPONENTSPOOLS_REST_ARGS(...) \
    EXPAND(GETCOMPONENTSPOOLS_FIRST_ARG(__VA_ARGS__))

#define GETCOMPONENTSPOOLS_REST_ARGS(...)

#define CONSTRUCTOR_ARGS_ELEMENT(component) \
    , Components::component##Ref *component \

#define CONSTRUCTOR_ARGS_FIRST_ARG(c, ...) CONSTRUCTOR_ARGS_ELEMENT(c) CONSTRUCTOR_ARGS_REST_ARGS(__VA_ARGS__)

#define CONSTRUCTOR_ARGS_REST_ARGS(...) \
    EXPAND(CONSTRUCTOR_ARGS_FIRST_ARG(__VA_ARGS__))

#define CONSTRUCTOR_ARGS_REST_ARGS(...)

#define CONSTRUCTOOR_DECL_ELEMENT(component) \
    _##component{component}, \

#define CONSTRUCTOR_DECL_FIRST_ARG(c, ...) CONSTRUCTOOR_DECL_ELEMENT(c) CONSTRUCTOR_DECL_REST_ARGS(__VA_ARGS__)

#define CONSTRUCTOR_DECL_REST_ARGS(...) \
    EXPAND(CONSTRUCTOR_DECL_FIRST_ARG(__VA_ARGS__))

#define CONSTRUCTOR_DECL_REST_ARGS(...)

#define DELETE_COMPONENTS_ELEMENT(component) \
    delete component; \

#define DELETE_COMPONENTS_FIRST_ARG(c, ...) DELETE_COMPONENTS_ELEMENT(c) DELETE_COMPONENTS_REST_ARGS(__VA_ARGS__)

#define DELETE_COMPONENTS_REST_ARGS(...) \
    EXPAND(DELETE_COMPONENTS_FIRST_ARG(__VA_ARGS__))

#define DELETE_COMPONENTS_REST_ARGS(...)

#define ACCESSORS_DECL_ELEMENT(name, component) \
    [[nodiscard]] Components::component##Ref *Entities::name##Ref::get##component() const \
    { \
        return _##component; \
    } \
    void Entities::name##Ref::set##component(const Components::component##Ref &component) \
    { \
        _##component = component; \
    } \
    \

#define ACCESSORS_DECL_FIRST_ARG(name, c, ...) ACCESSORS_DECL_ELEMENT(name, c) ACCESSORS_DECL_REST_ARGS(name, __VA_ARGS__)

#define ACCESSORS_DECL_REST_ARGS(name, ...) \
    EXPAND(ACCESSORS_DECL_FIRST_ARG(name, __VA_ARGS__))

#define ACCESSORS_DECL_REST_ARGS(name, ...)

#define _DECLARE_ENTITY_CPP(chunkSize, name, ...) \
    name##Ref::name##Ref( \
        Components::EntityStatusRef *status, \
        Components::ChunkPosRef *cPos \
        CONSTRUCTOR_ARGS_REST_ARGS(__VA_ARGS__) \
    ) : AEntityRef(status, cPos), \
        CONSTRUCTOR_DECL_REST_ARGS(__VA_ARGS__) \
    { \
    } \
    \
    name##Ref::~name##Ref() \
    { \
        DELETE_COMPONENTS_REST_ARGS(status, cPos, __VA_ARGS__) \
    } \
    \
    ACCESSORS_DECL_REST_ARGS(status, cPos, __VA_ARGS__) \
    \
    name##Pool::name##Pool() \
        : AEntityPool(#name, chunkSize) \
    { \
    } \
    \
    std::unique_ptr<IEntity> name##Pool::getEntity(Chunks::ChunkPos cPos) \
    { \
        return getRawEntity(cPos); \
    } \
    \
    std::unique_ptr<Entities::name> name##Pool::getRawEntity(Chunks::ChunkPos cPos) \
    { \
        auto ptr = std::make_unique<Entities::name>( \
            GETRAWENTS_REST_ARGS(__VA_ARGS__) \
        ); \
        return ptr; \
    } \
    \
    std::vector<Components::IComponentPool *> name##Pool::getComponentPools() \
    { \
        return { \
            GETCOMPONENTSPOOLS_REST_ARGS(__VA_ARGS__) \
        }; \
    }

#define DECLARE_ENTITY_CPP(chunkSize, name, ...) \
    _DECLARE_ENTITY_CPP(chunkSize, name, EntityStatus, ChunkPos, __VA_ARGS__)

    namespace ECS::Entities
    {
        class IEntityPool
        {
        public:
            IEntityPool() = default;
            virtual ~IEntityPool() = default;
            IEntityPool(const IEntityPool &other) = default;
            IEntityPool(IEntityPool &&other) = default;
            IEntityPool &operator=(const IEntityPool &other) = default;
            IEntityPool &operator=(IEntityPool &&other) = default;
            [[nodiscard]] virtual const std::string &getEntityName() const = 0;
            virtual std::unique_ptr<IEntityRef> getEntity(Chunks::ChunkPos cPos) = 0;
            virtual std::list<Chunks::ChunkPos> &getFreePos() = 0;
            virtual Chunks::chunk_pos_t getTotalSize() = 0;
            virtual Chunks::chunk_pos_t getUsedSize() = 0;
            [[nodiscard]] virtual Chunks::chunk_pos_t getChunkCount() const = 0;
            virtual std::vector<Components::IComponentPool *> getComponentPools() = 0;
            virtual Components::IComponentPool *getComponentPool(const std::string &name) = 0;
            virtual void addChunk() = 0;
        };
    }

