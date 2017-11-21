#!/bin/bash
# Utils - C++
cd utils/cpp
mkdir build
cd build
cmake .. -Dtests=ON
make
./UtilsTests
cd ../../..
# Utils - Javascript
cd utils/javascript
npm run test
./UtilsTests
cd ../..
