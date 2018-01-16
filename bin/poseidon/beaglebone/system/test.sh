#!/bin/bash
set -e
cd poseidon/beaglebone/system
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
./system_tests
cd ../../../..

echo Gathering coverage info.
{
coveralls --gcov-options '\-lp' -i ./poseidon/beaglebone/system -E ".*\/(extern|feature_tests|.+\.spec|CMake(C|CXX)CompilerId|test\/main|.+\.mock).+"
} &> /dev/null
echo Coverage info uploaded.
