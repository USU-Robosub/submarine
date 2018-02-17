#!/bin/bash
set -e

# compile tests
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
lcov --zerocounters  --directory .
./utils_arduino_tests

# generate coverage
echo Gathering coverage info.
{
lcov --directory . --capture --output-file utils_arduino.info
lcov -r utils_arduino.info /usr/include/\* -o utils_arduino.info
lcov -r utils_arduino.info \*/extern/\* -o utils_arduino.info
lcov -r utils_arduino.info \*.spec.\* -o utils_arduino.info
lcov -r utils_arduino.info \*.mock.\* -o utils_arduino.info
} &> /dev/null
genhtml --output-directory coverage --demangle-cpp --num-spaces 2 --sort --title "poseidon/arduino" --function-coverage --branch-coverage --legend utils_arduino.info
