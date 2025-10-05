#!/bin/bash
set -e  # stop if any command fails

# 1. Build the project
echo "Building project..."
make

# 2. Run the simulation
echo "Running simulation..."
./gravity_sim

# 3. Notify completion
echo "Simulation finished."

# 4. Run the Python plotting script
/home/jack/.pyenv/versions/engineering/bin/python \
    /home/jack/Documents/phd/newton-gravity-sim-cpp/plot_results.py