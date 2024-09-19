
# How to add a module to the engine

## 1. Create a new module

Create a new directory in the `modules` directory. The name of the directory should be the name of the module.

## 2. Create a `CMakeLists.txt` file

create a in `modules/<module-name>` directory. The content of the file should be similar to the following:

```cmake
cmake_minimum_required(VERSION 3.15)
set(MODULE_NAME <module-name>) # Change <module-name> to the name of the module
project(${MODULE_NAME})

add_library(${MODULE_NAME} STATIC lib.cpp) # Change the source file to the source file of the module

target_link_libraries(${MODULE_NAME} raylib) # link the needed libraries (ex: raylib, boost, etc...)

# Required for testing
enable_testing()
add_executable(${MODULE_NAME}_test tests/test.cpp) # Change test.cpp to the name of the test
target_link_libraries(${MODULE_NAME}_test Boost::unit_test_framework raylib)
add_test(NAME ${MODULE_NAME}Test COMMAND ${MODULE_NAME}_test)
```
Don't forget to change `<module-name>` to the name of the module and the source files.

## 3. Make the module available in the engine

Edit the `CMakeLists.txt` file in `engine/modules/` directory. Add the following lines to the top of the file:

```cmake
# ...
option(<module-name>_ENABLED "Enable <module-name> module" ON)
# ...
```

Add the following lines to the end of the file in `engine/modules/CMakeLists.txt`:

```cmake
# ...

if(${<module-name>_ENABLED})
    add_subdirectory(<module-name>)
endif()
```

## 4. Notes

- The module doesn't need to specify which other modules it depends on. This is the user's responsibility.
- The module should use it's own namespace to avoid conflicts with other modules.
- The module should be tested using the Boost unit test framework.
- The module should be documented using Doxygen comments.
- The module should be documented in the [modules](./modules.md) page.