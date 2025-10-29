#!/bin/bash

# This file runs examples and collects benchmark statistics
# on various demos.

# Delete the old benchmark file

rm bench.txt

perf stat -o bench.txt --append ./main.exe 100 2.269 100000
perf stat -o bench.txt --append ./main.exe 1024 2.269 100000000
perf stat -o bench.txt --append ./main.exe 1024 2.269 100000000000
