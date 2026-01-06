#!/bin/bash
module load intel-compilers/2025.2.0
make COMPILER=intel "$@"
