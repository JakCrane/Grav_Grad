#!/bin/bash
make clean
module load GCC/14.3.0
make COMPILER=amd "$@"