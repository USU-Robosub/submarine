#!/bin/bash
set -e

# get script's parent path
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

cd $parent_path/../poseidon

# start the ecosystem file that defines the poseidon apps
pm2 start ecosystem.config.js

# add to started on boot
pm2 save
