#!/bin/bash
set -e

# create test serial ports in background
socat PTY,link=/tmp/virtualcom0 PTY,link=/tmp/virtualcom1 & serial_port=$!

# compile tests
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
lcov --zerocounters  --directory .

# run serial port tests
./Utils_Tests [.] & tests=$!
wait $tests
kill $serial_port

# run normal tests
./Utils_Tests

# generate coverage
echo Gathering coverage info.
{
lcov --directory . --capture --output-file Utils.info
lcov -r Utils.info /usr/include/\* -o Utils.info
lcov -r Utils.info \*/extern/\* -o Utils.info
lcov -r Utils.info \*.spec.\* -o Utils.info
lcov -r Utils.info \*.mock.\* -o Utils.info
} &> /dev/null
genhtml --output-directory coverage --demangle-cpp --num-spaces 2 --sort --title "Utils" --function-coverage --branch-coverage --legend Utils.info
