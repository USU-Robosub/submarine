#!/bin/bash
set -e
# run tests
mkdir -p build
cd build
cmake .. -Dcoverage=OFF
make
./poseidon_arduino_Tests
