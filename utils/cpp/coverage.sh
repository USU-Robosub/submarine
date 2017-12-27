#!/bin/bash
set -e
# run tests
mkdir -p build
cd build
cmake .. -Dtests=ON -Dcoverage=ON
make
lcov --zerocounters  --directory .
./Utils_Tests
lcov --directory . --capture --output-file my_prog.info
lcov -r my_prog.info /usr/include/\* -o my_prog.info
genhtml --output-directory coverage --demangle-cpp --num-spaces 2 --sort --title "My Program's Test Coverage" --function-coverage --branch-coverage --legend my_prog.info
