#!/bin/bash

ROOT_DIR=$(git rev-parse --show-toplevel)

# Build the project
echo "Building the project..."
mkdir -p "$ROOT_DIR"/cmake-build-release
cmake -B "$ROOT_DIR"/cmake-build-release -S "$ROOT_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$ROOT_DIR"/cmake-build-release --parallel