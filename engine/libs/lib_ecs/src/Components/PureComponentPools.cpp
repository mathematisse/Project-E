/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base PureComponentPools
*/

#include "Components/PureComponentPools.hpp"


    namespace ECS::Components
    {
        EntityPoolIdPool::EntityPoolIdPool()
            : AComponentPool<entity_pool_id_t>("entity_pool_id")
        {
        }

        ChunkPosPool::ChunkPosPool()
            : AComponentPool2<Chunks::chunk_pos_t>("chunk_pos")
        {
        }

        EntityStatusPool::EntityStatusPool()
            : AComponentPool<entity_status_t>("entity_status")
        {
        }
    }

