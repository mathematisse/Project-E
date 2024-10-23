cmake_minimum_required(VERSION 3.15)
project(r-type_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RENDER_ENABLED ON)
set(NETWORK_ENABLED ON)
set(CORE_ENABLED ON)
set(SPATIAL2D_ENABLED ON)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(r-type_server ${SOURCES})

target_link_libraries(r-type_server r-type_common lib_ecs network core spatial2d render)
