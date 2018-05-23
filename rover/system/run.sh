#!/bin/bash
set -e
# build
mkdir -p build
cd build
cmake .. -Dtests=OFF -Dcoverage=OFF
make
./system
