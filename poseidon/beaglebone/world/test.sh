#!/bin/bash
set -e
# run tests
mkdir -p build
cd build
cmake .. -Dtests=ON
make
./world_tests