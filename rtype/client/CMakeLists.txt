cmake_minimum_required(VERSION 3.15)
project(r-type_client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RENDER_ENABLED ON)
set(PHYSICS_ENABLED ON)
set(ECS_ENABLED OFF)
set(NETWORK_ENABLED ON)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(r-type_client ${SOURCES})

# Get the root of the Git repository
execute_process(
    COMMAND git rev-parse --show-toplevel
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_ROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

include_directories(${GIT_ROOT}/.submodules/raygui/src)

target_link_libraries(r-type_client r-type_common render physics lib_ecs raylib network lib_log)

if(UNIX)
    target_link_libraries(r-type_client ncurses)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-enum-compare") #for raygui
    endif()
endif()