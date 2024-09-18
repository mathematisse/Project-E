cmake_minimum_required(VERSION 3.15)
project(RType)

# export COMPILE_COOMMANDS
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/engine Engine)

set(RENDER_ENABLED ON)
set(PHYSICS_ENABLED ON)
set(ECS_ENABLED OFF)

add_executable(RType main.cpp)

target_link_libraries(RType render physics)
target_include_directories(RType PRIVATE 
    ${CMAKE_SOURCE_DIR}/engine/modules/
)
