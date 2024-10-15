# Build the project
echo "Building the project..."
mkdir -p .\build
cmake -B .\build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build .\build
cp .\build\rtype\client\r-type_client.exe .
cp .\build\rtype\server\r-type_server.exe .