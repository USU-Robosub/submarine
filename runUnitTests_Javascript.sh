#!/bin/bash
set -e
# Utils
cd utils/javascript
npm install
# npm run test
cd ../..
node utils/javascript/node_modules/jest/bin/jest.js --config utils/javascript/config.json --coverage
cat utils/javascript/coverage/lcov.info | utils/javascript/node_modules/coveralls/bin/coveralls.js
