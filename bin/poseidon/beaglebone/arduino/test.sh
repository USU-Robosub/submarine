#!/bin/bash
set -e
cd poseidon/beaglebone/arduino
mkdir -p build
cd build
cmake .. -Dcoverage=ON
make
./poseidon_arduino_Tests
cd ../../../..

echo ===== Building with PlatformIO =====
platformio run

echo Gathering coverage info.
{
coveralls --gcov-options '\-lp' -i ./poseidon/beaglebone/arduino -E ".*\/(extern|feature_tests|.+\.spec|CMake(C|CXX)CompilerId|test\/main|.+\.mock).+"
} &> /dev/null
echo Coverage info uploaded.
