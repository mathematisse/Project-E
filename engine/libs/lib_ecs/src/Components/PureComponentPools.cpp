/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base CoreComponentPools
*/

#include "Components/PureComponentPools.hpp"


    namespace ECS::Components
    {

        template <>
        const std::string AComponentPool<entity_status_t>::componentName = "entity_status";

        
        template <>
        const std::string AComponentPool<Chunks::chunk_pos_t, Chunks::chunk_pos_t>::componentName = "chunk_pos";


        template <>
        const std::string AComponentPool<entity_pool_id_t>::componentName = "entity_pool_id";

    }

