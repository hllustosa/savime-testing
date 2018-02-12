#!/bin/bash

for i in {1..30}
do
	#low selectivy in a single chunk
	(time iquery -naq 'consume(between(sis, 0, 0, 0, 0, 0, 199, 450, 450));') >> ./times$i.txt  2>&1  

	#high selectivy in a single chunk
	(time iquery -naq 'consume(between(sis, 0, 0, 0, 0, 0, 150, 300, 300));') >> ./times$i.txt  2>&1  

	#low selectivy in a few chunks
	(time iquery -naq 'consume(between(sis, 0, 0, 0, 0, 100, 199, 450, 450));') >> ./times$i.txt  2>&1  

	#high selectivy in a few chunks
	(time iquery -naq 'consume(between(sis, 0, 0, 0, 0, 100, 150, 300, 300));') >> ./times$i.txt  2>&1  

	#low selectivy in a many chunks
	(time iquery -naq 'consume(between(sis, 0, 0, 0, 0, 500, 199, 450, 450));') >> ./times$i.txt  2>&1  

	#high selectivy in a many chunks
	(time iquery -naq 'consume(between(sis, 0, 0, 0, 0, 500, 150, 300, 300));') >> ./times$i.txt  2>&1  

	#exact query
	(time iquery -naq 'consume(between(sis, 0, 0, 0, 0, 0, 251, 501, 501));') >> ./times$i.txt  2>&1  

done
