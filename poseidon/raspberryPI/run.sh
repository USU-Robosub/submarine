#!/bin/bash
set -e
# build
mkdir -p build
cd build
cmake ..
make
./vision
