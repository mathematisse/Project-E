cmake_minimum_required(VERSION 3.15)
project(lib_net)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(lib_net STATIC lib.cpp)

target_link_libraries(lib_net)

# Required for testing
enable_testing()
add_executable(lnet_test tests/basic.cpp)
target_link_libraries(lnet_test Boost::unit_test_framework)
add_test(NAME LnetTest COMMAND Lnet_test)
