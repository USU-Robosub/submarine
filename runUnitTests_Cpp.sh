#!/bin/bash
set -e
# Utils
cd utils/cpp
mkdir build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
./UtilsTests
cd ../../..
coveralls -i utils/cpp --exclude utils/cpp/build
