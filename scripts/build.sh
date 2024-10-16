#!/bin/bash

ROOT_DIR=$(git rev-parse --show-toplevel)

# Build the project
echo "Building the project..."
mkdir -p "$ROOT_DIR"/build
cmake -B "$ROOT_DIR"/build -S "$ROOT_DIR" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$ROOT_DIR"/build --parallel
