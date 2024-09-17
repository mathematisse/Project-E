cmake_minimum_required(VERSION 3.15)
project(physics)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(physics STATIC physics.cpp)

target_link_libraries(physics Boost::asio)

target_include_directories(physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
