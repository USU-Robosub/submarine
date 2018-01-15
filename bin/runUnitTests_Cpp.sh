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
coveralls --gcov-options '\-lp' -i utils/cpp -E ".+\/(feature_tests|.+\.spec|CMake(C|CXX)CompilerId|test\/main|.+\.mock).+"

# ==================================================
# =                    poseidon                    =
# ==================================================

# ===================================
# =       beaglebone/system         =
# ===================================
cd poseidon/beaglebone/system
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
./system_tests
cd ../../../..
coveralls --gcov-options '\-lp' -i poseidon/beaglebone/system -E ".+\/(feature_tests|.+\.spec|CMake(C|CXX)CompilerId|test\/main|.+\.mock).+"

# ===================================
# =          raspberryPI            =
# ===================================
cd poseidon/raspberryPI
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
./raspberryPI_tests
cd ../../..
coveralls --gcov-options '\-lp' -i poseidon/raspberryPI -E ".+\/(feature_tests|.+\.spec|CMake(C|CXX)CompilerId|test\/main|.+\.mock).+"
