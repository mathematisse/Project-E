cmake_minimum_required(VERSION 3.15)
project(core)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_library(core STATIC Systems.cpp)
target_link_libraries(core lib_ecs)