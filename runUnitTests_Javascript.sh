#!/bin/bash
set -e
# Utils
cd utils/javascript
npm install
npm run test
cd ../..
