#!/bin/bash

iquery -nq "drop array sisRaw;"
iquery -nq "drop array sis;"
iquery -nq "create array sis <speed:float>[trial=0:*,1,0, x=0:*,251,0, y=0:*,501,0, z=0:*,501,0];"
iquery -nq "create array sisRaw <speed:float>[i=0:*,1000000,0];"

#Loading data one chunk at time
for INDEX in {0..499}
do
    CMD="iquery -nq \"load sisRaw from '../data/vp' as '(float)';\"" 
    eval $CMD

    #filtering dataset before redimensioning it in order to make it sparse	
    CMD="iquery -naq \"insert(redimension(filter(apply(sisRaw, trial, "$INDEX", x, i/251001, y, (i%251001)/501, z, i%501), x%2=1 and y%2=0), sis), sis);\""
    eval $CMD
    CMD="iquery -nq \"drop array sisRaw;\""
    eval $CMD
    CMD="iquery -nq \"create array sisRaw <speed:float>[i=0:*,1000000,0];\""
    eval $CMD
done


