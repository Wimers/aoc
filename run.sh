#!/bin/bash

echo "Building..."
make all > /dev/null

echo "Testing..."
ls *[p1,p2] | cat | while IFS= read -r line; do
	hyperfine -N --warmup 5 --max-runs 100 ./$line >> results
done

echo "Cleaning up..."
make clean > /dev/null
