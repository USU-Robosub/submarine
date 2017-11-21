#!/bin/bash
# Utils
cd utils/cpp
mkdir build
cd build
cmake .. -Dtests=ON
make
./UtilsTests
cd ../../..
