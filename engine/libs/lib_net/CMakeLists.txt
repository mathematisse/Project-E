cmake_minimum_required(VERSION 3.15)
project(lib_net)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lib_net STATIC lib.cpp)

file(GLOB_RECURSE LIB_NET_SOURCES "*.cpp")
target_sources(lib_net PRIVATE ${LIB_NET_SOURCES})

# target_link_libraries(lib_net ${boostincludedir})

if(MINGW)
  target_link_libraries(lib_net ws2_32 wsock32)
endif()

# Required for testing
# enable_testing()
# add_executable(lnet_test tests/basic.cpp)
# target_link_libraries(lnet_test ${Boost_LIBRARIES})
# add_test(NAME LnetTest COMMAND Lnet_test)
