#!/bin/bash

setsid savime -m 3 -b /home/hermano/Savime/etc/savime.boot > savime.log &
sleep 5
savimec 'create_tar("topology_1", "IncidenceTopology", "implicit, problem_id, int, 0, *, 1 | implicit, element, int, 0, *, 1", "element_point, long:4 | element_type, int", "problem_id, simulation, element, incidentee, element_point, incident, element_type, celltype");'
savimec 'create_tar("geometry_1", "CartesianGeometry", "implicit, problem_id, int, 0, *, 1 | implicit, point, long, 0, *, 1", "point_coords, double:2", "problem_id, simulation, point, index, point_coords, coords");'
savimec 'create_tar("solution_1", "UnstructuredFieldData", "implicit, problem_id, int, 0, *, 1 | implicit, simulation_time, int, 0, *, 1 | implicit, point, long, 0, *, 1", "solution, double:1", "problem_id, simulation, simulation_time, time, point, index");'
