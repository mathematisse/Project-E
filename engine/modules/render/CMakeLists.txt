cmake_minimum_required(VERSION 3.15)
project(render)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(render STATIC render.cpp)

target_link_libraries(render raylib)

target_include_directories(render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
