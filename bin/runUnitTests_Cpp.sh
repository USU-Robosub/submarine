#!/bin/bash
set -e
socat PTY,link=/tmp/virtualcom0 PTY,link=/tmp/virtualcom1 & serial_port=$!
# Utils
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
