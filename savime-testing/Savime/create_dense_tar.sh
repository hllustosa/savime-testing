#!/bin/bash

#Creating TAR in SAVIME
savimec 'create_tar("sis", "CartesianFieldData3d", "implicit,trial,int,0,1200,1 | implicit,x,int,0,250,1 | implicit,y,int,0,500,1 | implicit,z,int,0,500,1", "speed,float", "trial, simulation, x, x, y, y, z, z");'

for INDEX in {0..499}
do
    savimec 'create_dataset("vp'$INDEX':float", "'$(pwd)'../data/vp");'
    savimec 'load_subtar("sis", "ordered,trial, '$INDEX', '$INDEX' | ordered,x,0,250 | ordered,y,0,500 | ordered,z,0,500", "speed,vp'$INDEX'");'
done




