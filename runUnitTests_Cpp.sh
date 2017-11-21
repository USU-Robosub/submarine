#!/bin/bash
set -e
# Utils
cd utils/cpp
mkdir build
cd build
cmake .. -Dtests=ON
make
./UtilsTests
cd ../../..
coveralls -i utils/cpp
