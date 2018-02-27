#!/bin/bash
set -e

# compile tests
mkdir -p build
cd build
cmake .. -Dcoverage=ON
make
lcov --zerocounters  --directory .
./poseidon_arduino_tests

# generate coverage
echo Gathering coverage info.
{
lcov --directory . --capture --output-file poseidon_arduino.info
lcov -r poseidon_arduino.info /usr/include/\* -o poseidon_arduino.info
lcov -r poseidon_arduino.info \*/extern/\* -o poseidon_arduino.info
lcov -r poseidon_arduino.info \*.spec.\* -o poseidon_arduino.info
lcov -r poseidon_arduino.info \*.mock.\* -o poseidon_arduino.info
lcov -r poseidon_arduino.info \*/utils/arduino/\* -o poseidon_arduino.info
} &> /dev/null
genhtml --output-directory coverage --demangle-cpp --num-spaces 2 --sort --title "poseidon/arduino" --function-coverage --branch-coverage --legend poseidon_arduino.info
