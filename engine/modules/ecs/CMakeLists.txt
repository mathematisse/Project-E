cmake_minimum_required(VERSION 3.15)
project(ecs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ecs STATIC ecs.cpp)

target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
