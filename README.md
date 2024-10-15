<a id="readme-top"></a>

<!-- PROJECT HEADER -->
# R-Type

R-Type is a horizontal-scrolling shooter arcade game developed and published by Irem in 1987. This project aims to recreate a similar game with multiplayer capabilities.

## Table of Contents
<details>
  <summary>Click to expand</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributors">Contributors</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

## About The Project

R-Type is a horizontal-scrolling shooter arcade game developed and published by Irem in 1987. The player controls a space fighter named the R-9 to defend humanity against a mysterious powerful alien life-form known as the Bydo.

The goal of this project is to recreate a game similar to R-Type with multiplayer capabilities. The challenges are to create a standalone game engine that can handle multiple players using the UDP protocol and to create a game that is fun to play.

## Getting Started

### Dependencies/Requirements
- C++20
- Raylib 5.0 # fetch automatically if not found
- Boost 1.83 # fetched automatically if not found
- CMake 3.10 or higher

### Supported Platforms
- Windows
- macOS
- Linux

### Build Instructions
#### Linux
1. Clone the repository:
2. Build the project using CMake:
```sh
./scripts/build.sh
```

#### Windows
1. Clone the repository:
2. Open powershell
3. Build the project using CMake:
```powershell
.\scripts\build.ps1
```

If you can't run the script because of the execution policy, you can change it with the following command:
```powershell
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
```

## Usage

### Server
```sh
./r-type_server [port]
./r-type_client [ip] [port]
```

## Documentation

The documentation is available in the `docs` folder. You can read it [here](docs/src/SUMMARY.md).
use mdbook to open the documentation in your browser:
```sh
mdbook serve docs --open
```

## Contributors

- [Léo Wehrle - Rentmeister](https://github.com/leoWherle)
- [Théodore Magna](https://github.com/TheodoreMagna)
- [Lucien Pineau](https://github.com/mathematisse)
- [Alexis Hachemi](https://github.com/alexishachemi)
- [Ugo Blanc](https://github.com/HUMMMCHARAL)

## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

