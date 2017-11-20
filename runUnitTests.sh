#!/bin/bash
cd utils/cpp
mkdir build
cd build
cmake .. -Dtests=ON
make
./UtilsTests
