cmake_minimum_required(VERSION 3.15)
project(spatial2d)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(spatial2d STATIC ${SOURCES})
target_link_libraries(spatial2d lib_ecs)