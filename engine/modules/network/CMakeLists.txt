cmake_minimum_required(VERSION 3.15)
project(network)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(network STATIC network.cpp TestServer.cpp)

target_link_libraries(network lib_net)
