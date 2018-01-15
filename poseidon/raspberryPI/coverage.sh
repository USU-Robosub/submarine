#!/bin/bash
set -e

# compile tests
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
lcov --zerocounters  --directory .

# run tests
./raspberryPI_tests

# generate coverage
echo Gathering coverage info.
{
lcov --directory . --capture --output-file raspberryPI.info
lcov -r raspberryPI.info /usr/include/\* -o raspberryPI.info
lcov -r raspberryPI.info \*/extern/\* -o raspberryPI.info
lcov -r raspberryPI.info \*.spec.\* -o raspberryPI.info
lcov -r raspberryPI.info \*.mock.\* -o raspberryPI.info
} &> /dev/null
genhtml --output-directory coverage --demangle-cpp --num-spaces 2 --sort --title "raspberryPI" --function-coverage --branch-coverage --legend raspberryPI.info
