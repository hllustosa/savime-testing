/*
Compiling
g++ -o query_sparse query_sparse.cpp -O3 -fopenmp -std=c++11 `nc-config --cflags --libs`

Usage example
./query $(pwd)/sis.nc lower_trial lower_x lower_y lower_z upper_trial upper_x upper_y upper_z
*/

#include <stdio.h>
#include <string.h>
#include <netcdf.h>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string>
#include <time.h>
#include <omp.h>
#include <chrono>

using namespace std;

/* This is the name of the data file we will create. */
#define FILE_NAME "sis.nc"

#define NDIMS 2
#define NI 15718875
#define NTRIALS 500
#define SPEED_NAME "speed"

/* Names of things. */
#define NUM_PARAMS 10
#define NUM_THREADS 16
#define SPEED_NAME "speed"
#define X_NAME "x"
#define Y_NAME "y"
#define Z_NAME "z"
#define TRIAL_NAME "trial"

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERR(e) {printf("Error: %s\n", nc_strerror(e));}
#define ERR2(e) {printf("Error: %s\n", strerror(e));}

int main(int argc, char* args[])
{

   std::string input_file;
   int64_t low_trial, low_x, low_y, low_z, 
	   up_trial, up_x, up_y, up_z, 
           count_trial, count_x, count_y, count_z;
 
   /*Set input file path.*/
   if(argc < NUM_PARAMS)
   {
	printf("Not enough parameters.");
	return -1;
   }
   else
   {
	input_file = args[1];
	low_trial = atoi(args[2]);
	low_x = atoi(args[3]);
	low_y = atoi(args[4]);
	low_z = atoi(args[5]);

	up_trial = atoi(args[6]);
	up_x = atoi(args[7]);
	up_y = atoi(args[8]);
	up_z = atoi(args[9]);

	count_trial = up_trial - low_trial + 1;
	count_x = up_x - low_x + 1;
	count_y = up_y - low_y + 1;
	count_z = up_z - low_z + 1;

	printf("Reading file %s (%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld)\n", 
		input_file.c_str(), low_trial, low_x, low_y, low_z, 
		up_trial, up_x, up_y, up_z);
   }

   /* IDs for the netCDF file, dimensions, and variables. */
   int ncid, x_dimid, y_dimid, z_dimid, trial_dimid, i, error;
   int speed_varid, x_varid, y_varid, z_varid;
   int dimids[NDIMS];

   /* The start and count arrays will tell the netCDF library where to
      write our data. */
   size_t start[NDIMS], count[NDIMS];

   /* Error handling. */
   int retval;

   /* Create the file. */
   if ((retval = nc_open(input_file.c_str(), NC_NOWRITE, &ncid)))
      ERR(retval);

   if ((retval = nc_inq_varid(ncid, SPEED_NAME, &speed_varid)))
      ERR(retval);
  
   if ((retval = nc_inq_varid(ncid, X_NAME, &x_varid)))
      ERR(retval);
	
   if ((retval = nc_inq_varid(ncid, Y_NAME, &y_varid)))
      ERR(retval);

   if ((retval = nc_inq_varid(ncid, Z_NAME, &z_varid)))
      ERR(retval);

   int sizef = NI*sizeof(float);
   int sizet = NI*sizeof(int);

   count[0] = 1;
   count[1] = NI;
   start[0] = 0;
   start[1] = 0;

   /*Setting number of threads*/
   omp_set_num_threads(NUM_THREADS);


   #pragma omp parallel for
   for(int i = low_trial; i <= up_trial; i++)
   {
	start[0] = i;

	int fd = open("temp.data", O_CREAT | O_RDWR | O_APPEND, 0666);	
	int fx = open("x.data", O_CREAT | O_RDWR | O_APPEND, 0666);
	int fy = open("y.data", O_CREAT | O_RDWR | O_APPEND, 0666);	
	int fz = open("z.data", O_CREAT | O_RDWR | O_APPEND, 0666);	
	int ft = open(("filter"+to_string(i)+".data").c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);	

	if (fd == -1 || fx == -1 || fy == -1 || fz == -1 || ft == -1)
		ERR2(errno)

	if((error = ftruncate(fd, sizef))!= 0)
		ERR2(errno) 

	if((error = ftruncate(fx, sizet))!= 0)
		ERR2(errno) 
	
	if((error = ftruncate(fy, sizet))!= 0)
		ERR2(errno) 
	
	if((error = ftruncate(fz, sizet))!= 0)
		ERR2(errno)

	if((error = ftruncate(ft, sizet*4))!= 0)
		ERR2(errno)

	float * speed_out = (float*) mmap(0, sizef, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        int * x = (int*) mmap(0, sizet, PROT_READ | PROT_WRITE, MAP_SHARED, fx, 0);
	int * y = (int*) mmap(0, sizet, PROT_READ | PROT_WRITE, MAP_SHARED, fy, 0);
	int * z = (int*) mmap(0, sizet, PROT_READ | PROT_WRITE, MAP_SHARED, fz, 0);
	float * filtered = (float*) mmap(0, sizet*4, PROT_READ | PROT_WRITE, MAP_SHARED, fz, 0);

	if(speed_out == MAP_FAILED || speed_out == NULL
	 	  || x == MAP_FAILED || x == NULL
		  || y == MAP_FAILED || y == NULL
		  || z == MAP_FAILED || z == NULL
		  || filtered == MAP_FAILED || filtered == NULL
	  )
	      ERR2(errno) 

	if ((retval = nc_get_vara_float(ncid, speed_varid, start, 
			      count, speed_out)))
	      ERR(retval);

	if ((retval = nc_get_vara_int(ncid, x_varid, start, 
			      count, x)))
	      ERR(retval);

	if ((retval = nc_get_vara_int(ncid, y_varid, start, 
			      count, y)))
	      ERR(retval);

	if ((retval = nc_get_vara_int(ncid, z_varid, start, 
			      count, z)))
	      ERR(retval);
	
	int count = 0;
	/*Filtering data within range.*/
	for(int index = 0; index < NI; index++)
	{
		if(x[index] >= low_x && x[index] <= up_x)
		  if(y[index] >= low_y && y[index] <= up_y)
		    if(z[index] >= low_z && z[index] <= up_z)
		    {
			filtered[count++] = x[index];
			filtered[count++] = y[index];
			filtered[count++] = z[index];
			filtered[count++] = speed_out[index];
		    }
	}	
	
       close(fx); 
       close(fy); 
       close(fz);
       close(ft);
   }

 
   /* Close the file. */
   if ((retval = nc_close(ncid)))
      ERR(retval);
   
   

   printf("Finished!\n");
   return 0;
}

