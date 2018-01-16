#!/bin/bash
set -e
cd poseidon/raspberryPI
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON -DutilsLib=ON -DutilsLibPath="../../utils/cpp"
make
./raspberryPI_tests
cd ../../..

echo Gathering coverage info.
{
coveralls --gcov-options '\-lp' -i ./poseidon/beaglebone/system -E ".*\/(extern|feature_tests|.+\.spec|CMake(C|CXX)CompilerId|test\/main|.+\.mock).+"
} &> /dev/null
echo Coverage info uploaded.
