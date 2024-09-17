cmake_minimum_required(VERSION 3.15)
project(MultiProjectSetup)

option(BUILD_RTYPE "Build R-Type" ON)
option(BUILD_MAP_EDITOR "Build MapEditor" OFF) # cmake -S . -B build/ -DBUILD_MAP_EDITOR=ON

add_subdirectory(engine)

if(BUILD_RTYPE)
    add_subdirectory(rtype)
endif()

if(BUILD_MAP_EDITOR)
    add_subdirectory(map_editor)
endif()
