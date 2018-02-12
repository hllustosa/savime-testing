#!/bin/bash

g++ create.cpp -o create -std=c++11
savimec 'create_tar("sis", "CartesianFieldData3d", "implicit,trial,int,0,1200,1 | implicit,x,int,0,250,1 | implicit,y,int,0,500,1 | implicit,z,int,0,500,1", "speed,float", "trial, simulation, x, x, y, y, z, z");'
savimec 'create_dataset("x:int", "@'$(pwd)/../data'/x");'
savimec 'create_dataset("y:int", "@'$(pwd)/../data'/y");'
savimec 'create_dataset("z:int", "@'$(pwd)/../data'/z");'

for i in {0..499}
do
    ./create $INDEX 15718875
    savimec 'create_dataset("trial'$INDEX':int", "@'$(pwd)'/trial'$INDEX'");'
    savimec 'create_dataset("vp'$INDEX':float", "@'$(pwd)'/speed");'
    savimec 'load_subtar("sis", "total, trial, '$INDEX', '$INDEX', trial'$INDEX' | total, x, 0, 250, x | total, y, 0, 500, y | total, z, 0, 500, z", "speed,vp'$INDEX'");'
done





