#!/bin/bash

#Example: ./mhm_ensight.sh '-np 16 -host 10.0.3.150:8,10.0.3.151:8' /home/hermano/mhm/mhm_solver_lib/unit_test_data/tree
#Example: ./mhm_ensight.sh '-np 16 -host 10.0.3.150:8,10.0.3.151:8' /dev/shm/tree
cd /home/hermano/mhm/mhm_solver_lib/MHM_Transient
mpirun $1 ./hermano_mpi_dsparseLU_dmumps ../unit_test_data/savime_test 2
cd /home/hermano/mhm/savime_test/catalyst
time (mpiexec $1  pvpython $(pwd)/ensight_catalyst.py $2 $(pwd)/catalyst_grad.py > /dev/null)


