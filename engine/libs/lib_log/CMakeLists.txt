cmake_minimum_required(VERSION 3.15)
project(lib_log)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_library(lib_log STATIC log.cpp)
