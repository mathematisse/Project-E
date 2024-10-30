cmake_minimum_required(VERSION 3.15)
project(network)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(network STATIC Systems.cpp)

target_link_libraries(network lib_net)
