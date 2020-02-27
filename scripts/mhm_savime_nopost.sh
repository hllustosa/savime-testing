#!/bin/bash

#Example: ./mhm_savime_insitu.sh '-np 16 -host 10.0.3.150:8,10.0.3.151:8' 
cd /home/hermano/mhm/mhm_solver_lib/MHM_Transient
mpirun $1 ./hermano_mpi_savime_nopost_dsparseLU_dmumps ../unit_test_data/savime_test  '10.0.3.149:65000' '1' '1'
