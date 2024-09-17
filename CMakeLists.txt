cmake_minimum_required(VERSION 3.15)
project(MultiProjectSetup)

option(BUILD_RTYPE "Build R-Type" ON)
option(BUILD_MAP_EDITOR "Build MapEditor" OFF) # cmake -S . -B build/ -DBUILD_MAP_EDITOR=ON

enable_testing()

if(UNIX)
    option(ENABLE_ASAN "Enable AddressSanitizer" ON)
    option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" ON)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(ENABLE_ASAN)
    message(STATUS "AddressSanitizer enabled")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

if(ENABLE_UBSAN)
    message(STATUS "UndefinedBehaviorSanitizer enabled")
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

add_subdirectory(engine)

if(BUILD_RTYPE)
    add_subdirectory(rtype)
endif()

if(BUILD_MAP_EDITOR)
    add_subdirectory(map_editor)
endif()
