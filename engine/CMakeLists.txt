cmake_minimum_required(VERSION 3.15)
project(Engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(RENDER_ENABLED "Enable render module" ON)
option(PHYSICS_ENABLED "Enable physics module" ON)
option(ECS_ENABLED "Enable ecs module" ON)
option(NETWORK_ENABLED "Enable network module" ON)

include(FetchContent)

set(BOOST_VERSION 1.86.0)


# remove warnings temporarily to avoid warnings from raylib & boost
set(OLD_CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "")

set(BOOST_ENABLE_CMAKE ON)
find_package(Boost ${BOOST_VERSION} QUIET)
if (NOT Boost_FOUND)
    message(STATUS "Boost ${BOOST_VERSION} not found, fetching it")
    FetchContent_Declare(
        boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-${BOOST_VERSION}
    )
    FetchContent_GetProperties(boost)
    if (NOT boost_POPULATED)
        set(FETCHCONTENT_QUIET NO) # Display more information during the fetch process
        FetchContent_MakeAvailable(boost)
    endif()
    include_directories(${CMAKE_SOURCE_DIR}/libs/boost)
else()
    message(STATUS "Using local Boost installation=${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# From Raylib's example CMakeLists.txt
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        if (WAYLAND_ENABLED)
            set(BUILD_WITH_WAYLAND ON CACHE BOOL "" FORCE)
        endif()
    endif()
endif()

if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

include_directories(${CMAKE_SOURCE_DIR}/libs/raylib)
## End of raylib

#enable the warnings back
set(CMAKE_CXX_FLAGS ${OLD_CMAKE_CXX_FLAGS})

add_subdirectory(libs)

include_directories(libs)
include_directories(modules)

add_subdirectory(modules)
