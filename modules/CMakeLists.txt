function (add_module module)
    option(${module}_ENABLED "Enable ${module} module" OFF)
    if(NOT TARGET ${module})
        add_subdirectory(${module})
    endif()
endfunction()

add_module(core)
add_module(render)
add_module(network)
add_module(spatial2d)
