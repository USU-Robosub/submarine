#!/bin/bash
set -e
cd poseidon/arduino
mkdir -p build
cd build
cmake .. -Dcoverage=ON
make
./poseidon_arduino_tests
cd ..

echo ===== Building with PlatformIO =====
platformio run

cd ../..

echo Gathering coverage info.
{
coveralls --gcov-options '\-lp' -i ./poseidon/arduino -E ".*\/(extern|feature_tests|.+\.spec|CMake(C|CXX)CompilerId|test\/main|.+\.mock).+"
} &> /dev/null
echo Coverage info uploaded.
