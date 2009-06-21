#!/bin/bash

echo "Beginning test..."

for i in `ls *.exe`; do
	if [[ $i != *.cov.exe ]]; then
		./$i | diff -q ${i/%exe/out} -
	fi;
done;

echo "Test done. If there are reports of files differ, then something went wrong."
