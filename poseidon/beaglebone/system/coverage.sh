#!/bin/bash
set -e

# compile tests
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
lcov --zerocounters  --directory .

# run tests
./system_tests

# generate coverage
echo Gathering coverage info.
{
lcov --directory . --capture --output-file system.info
lcov -r system.info /usr/include/\* -o system.info
lcov -r system.info \*/extern/\* -o system.info
lcov -r system.info \*.spec.\* -o system.info
lcov -r system.info \*.mock.\* -o system.info
} &> /dev/null
genhtml --output-directory coverage --demangle-cpp --num-spaces 2 --sort --title "system" --function-coverage --branch-coverage --legend system.info
