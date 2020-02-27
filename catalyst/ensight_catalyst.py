from paraview.simple import *
from paraview import coprocessing
from paraview.vtk import vtkPVCatalyst as catalyst
from mpi4py import MPI
import vtk
import vtk.vtkPVPythonCatalystPython as pythoncatalyst
import paraview.simple
import paraview.vtk as vtk
import paraview
import vtk.vtkParallelCorePython
import os
import sys

global coProcessor

#Split work among MPI processes
def splitwork(nitens, nprocs):
  start = [None]*nprocs
  end = [None]*nprocs
  chunk = nitens/nprocs;

  if(nitens > nprocs):
    for i in range(0, nprocs-1):
      end[i] = (i + 1) * chunk
      start[i+1] = end[i]
    
    start[0] = 0;
    end[nprocs - 1] = nitens -1
  else:
    start[0] = 0
    end[0] = nitens -1

  return [start, end]

#Main
def main(argv):

  #Set Parameters  
  CASE_FILE = 'micromesh.case'
  BASE_DIR = argv[1]
  CATALYST_SCRIPT = argv[2]

  #Set MPI vars
  comm = MPI.COMM_WORLD
  comm = MPI.COMM_WORLD.Split_type(MPI.COMM_TYPE_SHARED)
  nprocs = comm.Get_size()
  rank = comm.Get_rank()
  print('Process '+str(rank)+' of '+str(nprocs)) 

  #List dirs in which ensight files are located
  if not os.path.isdir(BASE_DIR):
   return

  alldirs = []
  for item in os.listdir(BASE_DIR):
    if os.path.isdir(os.path.join(BASE_DIR, item)):
      alldirs.append(os.path.join(BASE_DIR, item))

  #Define work distribution
  split = splitwork(len(alldirs), nprocs)

  coprocessor = catalyst.vtkCPProcessor()
  pipeline = pythoncatalyst.vtkCPPythonScriptPipeline()
  pipeline.Initialize(CATALYST_SCRIPT)
  coprocessor.AddPipeline(pipeline)

  #Iterating local problems within every rank range
  for i in range(split[0][rank], split[1][rank]):
      subdir = alldirs[i]
      print('Rank ' +str(rank)+ ' processing dir ' + subdir)

      os.chdir(subdir)
      #reader = vtk.vtkGenericEnSightReader()
      reader = vtk.vtkEnSightGoldReader()
      reader.SetCaseFileName(subdir+'/'+CASE_FILE)
      reader.Update()
      timesteps = reader.GetTimeSets().GetItem(0)

      #For every local problem, iterating over timesteps
      for i in range(0, timesteps.GetSize()):

        timestep = timesteps.GetValue(i);
        reader.SetTimeValue(timestep)
        reader.Update()
        output = reader.GetOutput()
        grid = output.GetBlock(0)
        doCoprocessing = True

        if doCoprocessing:
        #coprocessor = catalyst.vtkCPProcessor()
        #pipeline = pythoncatalyst.vtkCPPythonScriptPipeline()
        #pipeline.Initialize(CATALYST_SCRIPT)
        #coprocessor.AddPipeline(pipeline)

          dataDescription = catalyst.vtkCPDataDescription()
          dataDescription.ForceOutputOn()
          dataDescription.AddInput('input')
          dataDescription.SetTimeData(timestep, i)
          dataDescription.GetInputDescriptionByName('input').SetGrid(grid)
          coprocessor.CoProcess(dataDescription)

	#if doCoprocessing:
	 # coprocessor.Finalize()

  coprocessor.Finalize()

if __name__ == "__main__":
    main(sys.argv)


