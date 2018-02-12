//g++ create.cpp -o create -std=c++11
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

main(int argc, char* args[])
{
	int val = atoi(args[1]);
	int size = atoi(args[2]);
	int * array = (int*)malloc(sizeof(int)*size);

	for(int i = 0; i < size; i++)
		array[i] = val;

	std::string file_name = "trial"+std::to_string(val);
	FILE * f = fopen(file_name.c_str(), "w");	
	fwrite((char*)array, size, sizeof(int), f);
	fclose(f);
}

