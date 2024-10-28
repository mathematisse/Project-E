cmake_minimum_required(VERSION 3.15)
project(r-type_common)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SPATIAL2D_ENABLED ON)
set(CORE_ENABLED ON)
set(RENDER_ENABLED ON)

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(r-type_common STATIC ${SOURCES})

target_link_libraries(r-type_common core raylib spatial2d render core)