#!/bin/bash
pkill 'hermano'
rm -r /dev/shm/tree

echo '1'
ssh  10.0.3.151  pkill hermano
ssh  10.0.3.151  rm -r /dev/shm/tree

echo '2'
ssh  10.0.3.152  pkill hermano
ssh  10.0.3.152  rm -r /dev/shm/tree

echo '3'
ssh  10.0.3.153  pkill hermano
ssh  10.0.3.153  rm -r /dev/shm/tree

echo '4'
ssh  10.0.3.154  pkill hermano
ssh  10.0.3.154  rm -r /dev/shm/tree

echo '5'
ssh  10.0.3.155  pkill hermano
ssh  10.0.3.155  rm -r /dev/shm/tree

echo '6'
#ssh  10.0.3.156  pkill hermano
#ssh  10.0.3.156  rm -r /dev/shm/tree

echo '7'
#ssh  10.0.3.157  pkill hermano
#ssh  10.0.3.157  rm -r /dev/shm/savime
