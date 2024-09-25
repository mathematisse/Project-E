
if(RENDER_ENABLED AND NOT TARGET render)
    add_subdirectory(render)
endif()

if(PHYSICS_ENABLED AND NOT TARGET physics)
    add_subdirectory(physics)
endif()

if(ECS_ENABLED AND NOT TARGET ecs)
    add_subdirectory(ecs)
endif()

if(NETWORK_ENABLED AND NOT TARGET network)
    add_subdirectory(network)
endif()
