#!/bin/bash
set -e
cd utils/arduino
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
./poseidon_arduino_tests

cd ../../..

echo Gathering coverage info.
{
coveralls --gcov-options '\-lp' -i ./utils/arduino -E ".*\/(extern|feature_tests|.+\.spec|CMake(C|CXX)CompilerId|test\/main|.+\.mock).+"
} &> /dev/null
echo Coverage info uploaded.
