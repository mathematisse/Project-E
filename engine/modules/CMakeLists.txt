if(CORE_ENABLED AND NOT TARGET core)
    add_subdirectory(core)
endif()

if(RENDER_ENABLED AND NOT TARGET render)
    add_subdirectory(render)
endif()

if(SPATIAL2D_ENABLED AND NOT TARGET spatial2d)
    add_subdirectory(spatial2d)
endif()

if(NETWORK_ENABLED AND NOT TARGET network)
    add_subdirectory(network)
endif()
