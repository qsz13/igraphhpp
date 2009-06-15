#!/bin/bash

echo "Beginning test..."

for i in `ls *.exe`; do
	./$i | diff -q ${i/%exe/out} -
done;

echo "Test done. If there are reports of files differ, then something went wrong."