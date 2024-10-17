cmake_minimum_required(VERSION 3.15)
project(r-type_common)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(r-type_common STATIC ${SOURCES})

target_link_libraries(r-type_common raylib)