#!/bin/bash
set -e
# Utils
cd utils/javascript
npm install
# npm run test
cd ../..
node utils/javascript/node_modules/jest/bin/jest.js --config config.json --coverage && cat ./coverage/lcov.info | ./node_modules/coveralls/bin/coveralls.js
