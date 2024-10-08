cmake_minimum_required(VERSION 3.15)
project(render)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(render STATIC render.cpp)

target_link_libraries(render raylib)

# Required for testing
enable_testing()
add_executable(render_test tests/render_test.cpp)
target_link_libraries(render_test${boostincludedir} raylib)
add_test(NAME RenderTest COMMAND render_test)
