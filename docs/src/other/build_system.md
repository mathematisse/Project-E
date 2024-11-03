
# How the build system works

- [Introduction](#introduction)
- [Dependencies](#dependencies)
    - [On linux:](#on-linux)
    - [On Windows:](#on-windows)
- [How to build](#how-to-build)
- [How to run](#how-to-run)
- [How to test](#how-to-test)

## Introduction

todo...

## Dependencies

The build system is based on [CMake](https://cmake.org/), a cross-platform build system generator. CMake is used to generate the build files for the project, which can be used to build the project on different platforms and with different compilers.

### On linux:


Ubuntu:
```bash
sudo apt install build-essential git
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```


Fedora:
```bash
sudo dnf groupinstall "Development Tools" "Development Libraries"
sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel libatomic
```

Arch Linux:
```bash
sudo pacman -S base-devel git
sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama
```

## How to build on Linux

### Using CMake:

#### Release build:

Configure the project:
```bash
cmake -B cmake-build-release -S . -DCMAKE_BUILD_TYPE=Release
```

Build the project:
```bash
cmake --build cmake-build-release --parallel --config Release
```

#### Debug build:

Configure the project:
```bash
cmake -B cmake-build-debug -S . -DCMAKE_BUILD_TYPE=Debug
```

Build the project:
```bash
cmake --build cmake-build-debug --parallel --config Debug
```

-----
*If you are using WSL2, you might have the following error:*
```bash
c++ fatal error killed signal terminated program cc1plus
```
You can fix this by removing the `--parallel` flag from the build command. (you can enable it again after the first build)

-----
You can also use the provided scripts to build the project:
```bash
./scripts/build.sh
```

## How to build on Windows

### Using CMake:

#### Release build:

Configure the project:
```bash
cmake -B cmake-build-release -S . -DCMAKE_BUILD_TYPE=Release
```

Build the project:
```bash
cmake --build cmake-build-release --config Release
```

#### Debug build:

Configure the project:
```bash
cmake -B cmake-build-debug -S . -DCMAKE_BUILD_TYPE=Debug
```

Build the project:
```bash
cmake --build cmake-build-debug --config Debug
```

-----
You can also use the provided scripts to build the project:
```powershell
.\scripts\build.ps1
```

## How to run

#### On linux:

The engine's libraries are located in the `cmake-build-release/engine` directory. And are directly linked to the binary. So you need to set the `LD_LIBRARY_PATH` environment variable to the `cmake-build-release/engine` directory.

Your binary will be located in the `cmake-build-release` or `cmake-build-debug` directory depending on the build type you chose.
The binary is standalone and can be run from anywhere. 
```bash
./cmake-build-release/<project>/<binary-name>
```


#### On Windows:

On Windows, the engine's libraries are located in the `build/engine` directory. And are directly linked to the binary. So you need to set the `PATH` environment variable to the `build/engine` directory.

Your binary will be located in the `build`.

The binary is standalone and can be run from anywhere. 
```powershell
.\build\<project>\<binary-name>.exe
```
or double-click on the executable, found in the root of the project.
