#!/usr/bin/env bash
cams="$(ls /dev/video*)"
for camera in $cams; do
	path="$(udevadm info --query=all $camera | grep -m 1 'P:' | cut -c3-)" 
	id="$(basename $(dirname $(dirname $path)) | cut -d: -f1)"
	echo "$id,$(basename $camera | cut -c6-)"
done
