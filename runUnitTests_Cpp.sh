--exclude#!/bin/bash
set -e
# Utils
cd utils/cpp
mkdir build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
./UtilsTests
cd ../../..
coveralls -i utils/cpp -e utils/cpp/build/CMakeFiles/feature_tests.cxx -e utils/cpp/build/CMakeFiles/feature_tests.c
