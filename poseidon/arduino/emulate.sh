#!/bin/bash
set -e
mkdir -p build
cd build
cmake ..
make
socat PTY,link=/tmp/virtualcom0 PTY,link=/tmp/virtualcom1 & serial_port=$!
./arduino /tmp/virtualcom0
kill $serial_port
