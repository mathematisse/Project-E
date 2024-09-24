cmake_minimum_required(VERSION 3.15)
project(r-type_client)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RENDER_ENABLED ON)
set(PHYSICS_ENABLED ON)
set(ECS_ENABLED OFF)
set(NETWORK_ENABLED ON)

add_executable(r-type_client main.cpp)

target_link_libraries(r-type_client r-type_common render physics ncurses)
