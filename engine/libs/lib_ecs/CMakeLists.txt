cmake_minimum_required(VERSION 3.15)
project(lib_ecs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(lib_ecs STATIC ${SOURCES})
