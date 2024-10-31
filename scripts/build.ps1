# Build the project
echo "Building the project..."
mkdir -p .\build
cmake -B .\build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build .\build
