#!/bin/bash
set -e

# ===================================
# =            Utils                =
# ===================================
socat PTY,link=/tmp/virtualcom0 PTY,link=/tmp/virtualcom1 & serial_port=$!
cd utils/cpp
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
./Utils_Tests [.] & tests=$!
wait $tests
kill $serial_port
./Utils_Tests
cd ../../..

# ==================================================
# =                    poseidon                    =
# ==================================================

# ===================================
# =       beaglebone/system         =
# ===================================
cd poseidon/beaglebone/system
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON -DutilsLib=ON -DutilsLibPath="../../../utils/cpp"
make
./system_tests
cd ../../../..

# ===================================
# =          raspberryPI            =
# ===================================
cd poseidon/raspberryPI
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON -DutilsLib=ON -DutilsLibPath="../../utils/cpp"
make
./raspberryPI_tests
cd ../../..

echo Gathering coverage info.
{
coveralls --gcov-options '\-lp' -i ./ -E "(extern|.+\/(feature_tests|.+\.spec|CMake(C|CXX)CompilerId|test\/main|.+\.mock)).+"
} &> /dev/null
echo Coverage info uploaded.
