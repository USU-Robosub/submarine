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
coveralls --gcov-options '\-lp' -i utils/cpp -E ".+\/(feature_tests|.+\CMake(C|CXX)CompilerId|test\/main|.+\.mock).+"
