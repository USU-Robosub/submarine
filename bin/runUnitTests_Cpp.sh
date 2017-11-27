#!/bin/bash
set -e
# Utils
cd utils/cpp
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
./Utils_Tests
cd ../../..
coveralls -i utils/cpp -E ".+\/(feature_tests|.+\.spec|CMake(C|CXX)CompilerId|test\/main|.+\.mock).+"
