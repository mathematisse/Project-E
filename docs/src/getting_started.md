# Getting Started

This section will guide you through the process of setting up the project and building it.

## Prerequisites

- [CMake](https://cmake.org/download/)
- [Git](https://git-scm.com/downloads)
- [A C++ compiler](https://gcc.gnu.org/)


**1. Setup the project**

```bash
git init
```

**2. Add Project-E as a submodule**

```bash
git submodule add https://mathematisse/Project-E engine
```

**3. Add the engine to your Project**

basic CMakeLists.txt:
```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)

set(RENDER_ENABLED ON)
set(PHYSICS_ENABLED ON)
set(ECS_ENABLED OFF)
set(NETWORK_ENABLED ON)

# Compile the project

add_subdirectory(engine)
target_link_libraries(MyProject PRIVATE core spatial2d render network)
```

