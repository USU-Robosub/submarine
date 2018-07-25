#!/bin/bash
set -e

# get script's parent path
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

cd $parent_path/../extern/cloud9

# start the ecosystem file that defines the cloud9 script
pm2 start ecosystem.config.js

# add to started on boot
pm2 save
