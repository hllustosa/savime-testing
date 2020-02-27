#!/bin/bash
pkill 'savime'
rm -r /dev/shm/savime

echo '1'
ssh  10.0.3.151  pkill savime
ssh  10.0.3.151  rm -r /dev/shm/savime

echo '2'
ssh  10.0.3.152  pkill savime
ssh  10.0.3.152  rm -r /dev/shm/savime

echo '3'
ssh  10.0.3.153  pkill savime
ssh  10.0.3.153  rm -r /dev/shm/savime

echo '4'
ssh  10.0.3.154  pkill savime
ssh  10.0.3.154  rm -r /dev/shm/savime

echo '5'
ssh  10.0.3.155  pkill savime
ssh  10.0.3.155  rm -r /dev/shm/savime

echo '6'
ssh  10.0.3.156  pkill savime
ssh  10.0.3.156  rm -r /dev/shm/savime

echo '7'
ssh  10.0.3.157  pkill savime
ssh  10.0.3.157  rm -r /dev/shm/savime
