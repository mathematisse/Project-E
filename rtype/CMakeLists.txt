cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/engine Engine)

include_directories( 
    ${CMAKE_SOURCE_DIR}/engine/modules/
    common
)

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)