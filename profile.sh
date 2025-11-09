#!/bin/bash

# This script will profile a specific command.
# We just use perf for this...

COM='./main.exe 1024 2.269 100000000'

perf record --freq=100 -g ${COM}
