#!/bin/bash
set -e
# run tests
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=OFF
make
./raspberryPI_tests