cmake_minimum_required(VERSION 3.15)
project(r-type_client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RENDER_ENABLED ON)
set(CORE_ENABLED ON)
set(SPATIAL2D_ENABLED ON)

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

target_link_libraries(r-type_client r-type_common render lib_ecs lib_net raylib core spatial2d lib_log)
