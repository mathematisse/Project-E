
# How to add a module to the engine

## 1. Create a new module

Create a new directory in the `modules` directory. The name of the directory should be the name of the module.

## 2. Create a `CMakeLists.txt` file

create a in `modules/<module-name>` directory. The content of the file should be similar to the following:

```cmake
cmake_minimum_required(VERSION 3.15)
project(<module-name>) # Swap <module-name> with the name of your module

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(spatial2d STATIC Systems.cpp) # Change the source file to the source file of the module
target_link_libraries(spatial2d lib_ecs) # link the needed libraries (ex: raylib, etc...)
```

Don't forget to change `<module-name>` to the name of the module and the source files.

## 3. Make the module available in the engine

Edit the `CMakeLists.txt` file in `engine/modules/` directory. Add the following lines to the top of the file:

```cmake
add_module(<module-name>)
```

## 4. Notes

- The module doesn't need to specify which other modules it depends on. This is the user's responsibility.
- The module should be documented using Doxygen comments.
- The module should have its own markdown file documenting it. The file should be listed to the [registry](registry.md) using a similar format with a hyperlink to the file.