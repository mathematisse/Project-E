#!/bin/bash

ROOT_DIR=$(git rev-parse --show-toplevel)

# Build the project
echo "Building the project..."
set -xe
git submodule update --init --recursive
mkdir -p "$ROOT_DIR"/build
cmake -B "$ROOT_DIR"/build -S "$ROOT_DIR" -DCMAKE_BUILD_TYPE=Debug -Drender_ENABLED=ON -Dspatial2d_ENABLED=ON -Dnetwork_ENABLED=ON -Dcore_ENABLED=ON
cmake --build "$ROOT_DIR"/build --parallel
