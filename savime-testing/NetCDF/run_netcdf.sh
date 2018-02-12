#!/bin/bash

#Uncomment line below for queries in dense files
#g++ -o range query.cpp -std=c++11  `nc-config --cflags --libs`

#Uncomment line below for queries in sparse files
#g++ -o range query_sparse.cpp -std=c++11  `nc-config --cflags --libs`

#Queries in netCDF
for i in {1..30}
do
	# selectivy in single chunk
	(time ./range $(pwd)/sis.nc 0 0 0 0 0 199 450 450) >> ./times$i.txt  2>&1  
	rm temp.data
	rm filter*

	#high selectivy in single chunk
	(time ./range $(pwd)/sis.nc 0 0 0 0 0 150 300 300) >> ./times$i.txt  2>&1  
	rm temp.data
	rm filter*

	#low selectivy in few chunks
	(time ./range $(pwd)/sis.nc 0 0 0 0 100 199 450 450) >> ./times$i.txt  2>&1 
	rm temp.data
	rm filter*

	#high selectivy in few chunks
	(time ./range $(pwd)/sis.nc 0 0 0 0 100 150 300 300) >> ./times$i.txt  2>&1  
	rm temp.data
	rm filter*

	#low selectivy in many chunks
	(time ./range $(pwd)/sis.nc 0 0 0 0 499 199 450 450) >> ./times$i.txt  2>&1 
	rm temp.data
	rm filter*

	#high selectivy in many chunks
	(time ./range $(pwd)/sis.nc 0 0 0 0 499 150 300 300) >> ./times$i.txt  2>&1  
	rm temp.data
	rm filter*

	#exact query
	(time ./range $(pwd)/sis.nc 0 0 0 0 0 250 500 500) >> ./times$i.txt  2>&1  
	rm temp.data
	rm filter*
done
