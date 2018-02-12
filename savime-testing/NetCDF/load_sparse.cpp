/*
Compiling
g++ -o load_sparse load_sparse.cpp -std=c++11  `nc-config --cflags --libs`

Usage example
./load_sparse (DATA_DIR)/speed (DATA_DIR)/x (DATA_DIR)/y (DATA_DIR)/z
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
#define NTRIALS 500 //Change for different file sizes
#define SPEED_NAME "speed"

/* Names of things. */
#define NUM_PARAMS 5
#define SPEED_NAME "speed"
#define X_NAME "x"
#define Y_NAME "y"
#define Z_NAME "z"
#define I_NAME "i"
#define TRIAL_NAME "trial"

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); return 2;}
#define ERR2(e) {printf("Error: %s\n", strerror(e)); return 2;}

int main(int argc, char* args[])
{

   std::string input_file, x_file, y_file, z_file;

   if(argc == NUM_PARAMS)
   {
        input_file = args[1];
	x_file = args[2];
	y_file = args[3];
	z_file = args[4];
   }
   else
   {
	printf("Not enough parameters.\n");	
	return -1;
   }

   /* IDs for the netCDF file, dimensions, and variables. */
   int ncid, i_dimid, trial_dimid, i;
   int speed_varid, x_varid, y_varid, z_varid;
   int dimids[NDIMS];

   /* The start and count arrays will tell the netCDF library where to
      write our data. */
   size_t start[NDIMS], count[NDIMS];
   size_t chunksize[NDIMS] = {1, NI};

   /* Error handling. */
   int retval;

   /* Create the file. */
   if ((retval = nc_create(FILE_NAME, NC_NETCDF4, &ncid)))
      ERR(retval);

   /* Define the dimensions. The record dimension is defined to have
    * unlimited length - it can grow as needed. In this example it is
    * the trial dimension.*/
   if ((retval = nc_def_dim(ncid, I_NAME, NI, &i_dimid)))
      ERR(retval);

   if ((retval = nc_def_dim(ncid, TRIAL_NAME, NC_UNLIMITED, &trial_dimid)))
      ERR(retval);

   /* The dimids array is used to pass the dimids of the dimensions of
      the netCDF variables. Both of the netCDF variables we are
      creating share the same four dimensions. In C, the
      unlimited dimension must come first on the list of dimids. */
   dimids[0] = trial_dimid;
   dimids[1] = i_dimid;

   /* Define the netCDF variables for the speed, x, y and z
    * data. */
   if ((retval = nc_def_var(ncid, SPEED_NAME, NC_FLOAT, NDIMS, 
                dimids, &speed_varid)))
      ERR(retval);

   if ((retval = nc_def_var(ncid, X_NAME, NC_INT, NDIMS, 
                dimids, &x_varid)))
      ERR(retval);

   if ((retval = nc_def_var(ncid, Y_NAME, NC_INT, NDIMS, 
                dimids, &y_varid)))
      ERR(retval);

   if ((retval = nc_def_var(ncid, Z_NAME, NC_INT, NDIMS, 
                dimids, &z_varid)))
      ERR(retval);

   if ((retval = nc_def_var_chunking(ncid, speed_varid, NC_CHUNKED, chunksize)))
      ERR(retval);      

   /* End define mode. */
   if ((retval = nc_enddef(ncid)))
      ERR(retval);

   /* These settings tell netcdf to write one timestep of data. (The
     setting of start[0] inside the loop below tells netCDF which
     timestep to write.) */
   count[0] = 1;
   count[1] = NI;
   start[1] = 0;

   /* Write the data.*/
   for (i = 0; i < NTRIALS; i++)
   {

      start[0] = i;
      int fd = open(input_file.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
      int fx = open(x_file.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);     
      int fy = open(y_file.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);     
      int fz = open(z_file.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);          
      
      if (fd == -1 || fx == -1 || fy == -1 || fz == -1)
        ERR2(errno)

      float * speed_out = (float*) mmap(0, NI*sizeof(float), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
      if(speed_out == MAP_FAILED || speed_out == NULL)
        ERR2(errno) 

      int * x = (int*) mmap(0, NI*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fx, 0);
      if(x == MAP_FAILED || x == NULL)
        ERR2(errno)

      int * y = (int*) mmap(0, NI*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fy, 0);
      if(y == MAP_FAILED || y == NULL)
        ERR2(errno) 	    

      int * z = (int*) mmap(0, NI*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fz, 0);
      if(z == MAP_FAILED || z == NULL)
        ERR2(errno) 

      if ((retval = nc_put_vara_float(ncid, speed_varid, start, count, 
                      speed_out)))
        ERR(retval);


      if ((retval = nc_put_vara_int(ncid, x_varid, start, count, 
                      x)))
        ERR(retval);
   

      if ((retval = nc_put_vara_int(ncid, y_varid, start, count, 
                      y)))
        ERR(retval);

      if ((retval = nc_put_vara_int(ncid, z_varid, start, count, 
                      z)))
        ERR(retval);	
 
      munmap(speed_out, NI*sizeof(float));
      munmap(x, NI*sizeof(float));        
      munmap(y, NI*sizeof(float));        
      munmap(z, NI*sizeof(float));                
      close(fd); close(fx); close(fy); close(fz);
   }

   /* Close the file. */
   if ((retval = nc_close(ncid)))
      ERR(retval);


   printf("Finished\n");
   return 0;
}









