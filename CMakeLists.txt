cmake_minimum_required(VERSION 3.15)
project(MultiProjectSetup)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_RTYPE "Build R-Type" ON)

enable_testing()

if(UNIX)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    endif()
    if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
        option(ENABLE_ASAN "Enable AddressSanitizer" ON)
        option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" ON)
    else()
        set(ENABLE_ASAN OFF)
        set(ENABLE_UBSAN OFF)
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type not set, default to Debug")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(ENABLE_ASAN)
    message(STATUS "AddressSanitizer enabled")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

if(ENABLE_UBSAN)
    message(STATUS "UndefinedBehaviorSanitizer enabled")
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

# add clang-tidy
# find_program(CLANG_TIDY clang-tidy)
# if(CLANG_TIDY)
#     set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY}")
# endif()

include_directories(engine/libs)
add_subdirectory(engine)

if(BUILD_RTYPE)
    add_subdirectory(rtype)
endif()
