#!/bin/bash
mpiexec -np $1 pvpython $(pwd)/ensight_catalyst.py $2 $(pwd)/catalyst_grad.py

