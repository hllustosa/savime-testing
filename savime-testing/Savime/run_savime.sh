#!/bin/bash

for i in {1..30}
do

	#low selectivity in a single chunk
	(time client 'scan(subset(sis, trial, 0, 0, x, 0, 199, y, 0, 450, z, 0, 450));') >> ./times$i.txt  2>&1 

	#high selectivity in a single chunk
	(time client 'scan(subset(sis, trial, 0, 0, x, 0, 150, y, 0, 300, z, 0, 300));') >> ./times$i.txt  2>&1 

	#low selectivity in a few chunks
	(time client 'scan(subset(sis, trial, 0, 100, x, 0, 199, y, 0, 450, z, 0, 450));') >> ./times$i.txt  2>&1 

	#high selectivity in a few chunks
	(time client 'scan(subset(sis, trial, 0, 100, x, 0, 150, y, 0, 300, z, 0, 300));') >> ./times$i.txt  2>&1 

	#low selectivity in many chunks
	(time client 'scan(subset(sis, x, 0, 199, y, 0, 450, z, 0, 450));') >> ./times$i.txt  2>&1 

	#high selectivity in many chunks
	(time client 'scan(subset(sis, x, 0, 150, y, 0, 300, z, 0, 300));') >> ./times$i.txt  2>&1 

	#exact query
	(time client 'scan(subset(sis, trial, 0, 0, x, 0, 251, y, 0, 501, z, 0, 501));') >> ./times$i.txt  2>&1 

done
