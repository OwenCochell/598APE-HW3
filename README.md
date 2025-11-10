# For Reviewers

This project can run without any extra dependencies, only a modern compiler is necessary.

The times of each stage will be printed to standard output, and the results of the operation will be placed in the `output` directory. We run the three scenarios as described later in this readme.

We also provide a simple script, `bench.sh`, which will run each scenario and print the times to `bench.txt`. `profile.sh` runs a scenario via perf to get profling info. Once the profile is complete, you can see the report via `perf report`.

Finally, to see notes on each change and the relevant benchmark info, have a look at `changes.md`. It contains all the improvements and their git IDs, so you can seek back to the codebase when that change was implemented to preform testing. Please note that every time you checkout to a commit, you must rebuild the docker image!

# 598APE-HW3

This repository contains code for homework 3 of 598APE.

This assignment is relatively simple in comparison to HW1 and HW2 to ensure you have enough time to work on the course project.

In particular, this repository implements a 2D Ising model Monte Carlo simulator (with Metropolis–Hastings algorithm) on an L×L lattice with periodic boundary conditions.

To compile the program run:
```bash
make -j
```

To clean existing build artifacts run:
```bash
make clean
```

This program assumes the following are installed on your machine:
* A working C compiler (gcc is assumed in the Makefile)
* make
* ImageMagick `convert` for PNG output

Usage (after compiling):
```bash
./main.exe <L> <temperature> <steps>
```

In particular, consider speeding up simple run like the following (which runs ~5 seconds on my local laptop under the default setup):
```bash
./main.exe 100 2.269 100000
```

Exact bitwise reproducibility is not required; sanity checks on energy/magnetization bounds must pass. In addition, at the critical temperature (T ≈ 2.269) the energy per spin should approach -1.414 in equilibrium.