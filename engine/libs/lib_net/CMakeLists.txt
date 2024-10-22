cmake_minimum_required(VERSION 3.15)
project(lib_net)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_library(lib_net STATIC "")

file(GLOB_RECURSE LIB_NET_SOURCES "*.cpp")
file(GLOB_RECURSE EXAMPLE_SOURCES "examples/*.cpp")

list(REMOVE_ITEM LIB_NET_SOURCES ${EXAMPLE_SOURCES})

target_sources(lib_net PRIVATE ${LIB_NET_SOURCES})

if(MINGW)
  target_link_libraries(lib_net ws2_32 wsock32)
endif()

project(lib_net_example)

add_executable(lib_net_example)

target_sources(lib_net_example PRIVATE ${EXAMPLE_SOURCES})

target_link_libraries(lib_net_example lib_net)


# Required for testing
# enable_testing()
# add_executable(lnet_test tests/basic.cpp)
# target_link_libraries(lnet_test ${Boost_LIBRARIES})
# add_test(NAME LnetTest COMMAND Lnet_test)

# enable_testing()
# add_executable(lib_net_test tests/basic.cpp) # Ensure this points to your test file
# target_sources(lib_net_test PRIVATE ${LIB_NET_SOURCES})
# target_compile_definitions(lib_net_test PRIVATE TEST_SECTION)
# target_link_libraries(lib_net_test GTest::gtest_main)
# target_include_directories(lib_net_test PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# include(GoogleTest)
# gtest_discover_tests(lib_net_test)