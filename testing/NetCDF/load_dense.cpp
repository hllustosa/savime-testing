/*
Compiling
g++ -o load_dense load_dense.cpp -std=c++11  `nc-config --cflags --libs`

Usage example
./load_dense (DATA_DIR)/vp
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

   /*Set input file path.*/
   if(argc > 1)
   {
	input_file = args[1];
   }
   else
   {
	input_file = "vp.bin";
   }

   /* IDs for the netCDF file, dimensions, and variables. */
   int ncid, x_dimid, y_dimid, z_dimid, trial_dimid, i;
   int speed_varid;
   int dimids[NDIMS];

   /* The start and count arrays will tell the netCDF library where to
      write our data. */
   size_t start[NDIMS], count[NDIMS];
   size_t chunksize[NDIMS] = {1, 251, 501, 501};

   /* Error handling. */
   int retval;

   /* Create the file. */
   if ((retval = nc_create(FILE_NAME, NC_NETCDF4, &ncid)))
      ERR(retval);

   /* Define the dimensions. The record dimension is defined to have
    * unlimited length - it can grow as needed. In this example it is
    * the trial dimension.*/
   if ((retval = nc_def_dim(ncid, X_NAME, NX, &x_dimid)))
      ERR(retval);
   if ((retval = nc_def_dim(ncid, Y_NAME, NY, &y_dimid)))
      ERR(retval);
   if ((retval = nc_def_dim(ncid, Z_NAME, NZ, &z_dimid)))
      ERR(retval);
   if ((retval = nc_def_dim(ncid, TRIAL_NAME, NC_UNLIMITED, &trial_dimid)))
      ERR(retval);

   /* The dimids array is used to pass the dimids of the dimensions of
      the netCDF variables. Both of the netCDF variables we are
      creating share the same four dimensions. In C, the
      unlimited dimension must come first on the list of dimids. */
   dimids[0] = trial_dimid;
   dimids[1] = x_dimid;
   dimids[2] = y_dimid;
   dimids[3] = z_dimid;

   /* Define the netCDF variables for the pressure and temperature
    * data. */
   if ((retval = nc_def_var(ncid, SPEED_NAME, NC_FLOAT, NDIMS, 
                dimids, &speed_varid)))
      ERR(retval);

   /*Defining chunking configuration. */
   if ((retval = nc_def_var_chunking(ncid, speed_varid, NC_CHUNKED, chunksize)))
      ERR(retval);  

   /* End define mode. */
   if ((retval = nc_enddef(ncid)))
      ERR(retval);

   count[0] = 1;
   count[1] = NX;
   count[2] = NY;
   count[3] = NZ;
   start[1] = 0;
   start[2] = 0;
   start[3] = 0;

   /* Write the data one chunk at a time.*/
   for (i = 0; i < NTRIALS; i++)
   {
      start[0] = i;

      int fd = open(input_file.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);	
      if (fd == -1)
        ERR2(errno)

      float * speed_out = (float*) mmap(0, NX*NY*NZ*sizeof(float), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
      if(speed_out == MAP_FAILED || speed_out == NULL)
        ERR2(errno) 

      if ((retval = nc_put_vara_float(ncid, speed_varid, start, count, 
                      speed_out)))
      ERR(retval);

      munmap(speed_out, NX*NY*NZ*sizeof(float));        
      close(fd);
   }

   /* Close the file. */
   if ((retval = nc_close(ncid)))
      ERR(retval);
   
   
   printf("Finished!\n");
   return 0;
}

