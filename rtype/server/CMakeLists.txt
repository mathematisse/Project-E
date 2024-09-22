cmake_minimum_required(VERSION 3.15)
project(r-type_server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RENDER_ENABLED OFF)
set(PHYSICS_ENABLED ON)
set(ECS_ENABLED ON)

add_executable(r-type_server main.cpp)

target_link_libraries(r-type_server r-type_common ecs physics)
