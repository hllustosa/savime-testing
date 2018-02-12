/*
Compiling
g++ -o query query.cpp -O3 -std=c++11 `nc-config --cflags --libs`

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

/* Writing 4D data, a 251 x 501 x 501 x 500 seismiec dataset */
#define NDIMS 4
#define NX 251
#define NY 501
#define NZ 501
#define NTRIALS 500
#define SPEED_NAME "speed"

/* Names of things. */
#define NUM_PARAMS 10
#define SPEED_NAME "speed"
#define X_NAME "x"
#define Y_NAME "y"
#define Z_NAME "z"
#define TRIAL_NAME "trial"

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); return 2;}
#define ERR2(e) {printf("Error: %s\n", strerror(e)); return 2;}

int main(int argc, char* args[])
{

   std::string input_file;
   int64_t low_trial, low_x, low_y, low_z, count_trial, count_x, count_y, count_z;
   
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
	
	count_trial = atoi(args[6]) - low_trial + 1;
	count_x = atoi(args[7]) - low_x + 1;
	count_y = atoi(args[8]) - low_y + 1;
	count_z = atoi(args[9]) - low_z + 1;

	printf("Reading file %s (%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld)\n", 
				input_file.c_str(), low_trial, low_x, low_y, low_z, 
				atol(args[6]), atol(args[7]), atol(args[8]), atol(args[9]));
	
   }

   /* IDs for the netCDF file, dimensions, and variables. */
   int ncid, x_dimid, y_dimid, z_dimid, trial_dimid, i, error;
   int speed_varid;
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
   
   count[0] = count_trial;
   count[1] = count_x;
   count[2] = count_y;
   count[3] = count_z;
   start[0] = low_trial;
   start[1] = low_x;
   start[2] = low_y;
   start[3] = low_z;

   /*Calculating total extracted hyperslab size.*/	
   int64_t size = count_x*count_y*count_z*count_trial*sizeof(float);

   int fd = open("temp.data", O_CREAT | O_RDWR | O_APPEND, 0666);	
   if (fd == -1)
      ERR2(errno)

   if((error = ftruncate(fd, size))!= 0)
      ERR2(errno) 

   float * speed_out = (float*) mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if(speed_out == MAP_FAILED || speed_out == NULL)
      ERR2(errno) 

   /*Extracting data from file.*/
   if ((retval = nc_get_vara_float(ncid, speed_varid, start, 
		      count, speed_out)))
	 ERR(retval);


   munmap(speed_out, size);        
   close(fd);
   
   /* Close the file. */
   if ((retval = nc_close(ncid)))
      ERR(retval);
   
  
   printf("Finished\n");
   return 0;
}

