#!/bin/bash

strides=(4 64 128 1024 1048576 4194304 16777216)

for stride in ${strides[@]}; do
    echo "Running for stride length: $stride"
    sudo taskset -c 1 ./bin/memaccesstime $stride > "results/memaccess_$stride"
done
