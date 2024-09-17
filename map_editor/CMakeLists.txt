cmake_minimum_required(VERSION 3.15)
project(MapEditor)

add_subdirectory(${CMAKE_SOURCE_DIR}/engine Engine)

set(RENDER_ENABLED ON)
set(PHYSICS_ENABLED OFF)
set(ECS_ENABLED ON)

add_executable(MapEditor main.cpp)

target_link_libraries(MapEditor render ecs)
target_include_directories(MapEditor PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/modules/
)
