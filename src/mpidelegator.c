#include <stdio.h>
#include <mpi.h>
#include "common.h"
#include "mpidelegator.h"

MPI_Datatype MPI_WayPoint;

void mpiStructInit()
{
	MPI_Datatype MPI_Types[4] = { MPI_FLOAT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT };
	int blocklengths[4] = {1,1,1,1};
	MPI_Aint displacements[4] = {0, sizeof(float), sizeof(float) *2, sizeof(float) * 3};
	MPI_Type_struct(4, blocklengths, displacements, MPI_Types, &MPI_WayPoint);
	MPI_Type_commit(&MPI_WayPoint);
}

void hostNode(int size)
{
	WayPoint **data = readData("flights", "outfile", NUM_FLIGHTS, NUM_WAYPOINTS);
  MPI_Bcast(&data[0][0], NUM_FLIGHTS * NUM_WAYPOINTS, MPI_WayPoint, 0, MPI_COMM_WORLD);
	calculateDistances(data, ENDPOINTS[size-1]-1, 0);
}

void clientNode(int rank, int size)
{
	WayPoint **data = allocate2DWayPoints(NUM_FLIGHTS, NUM_WAYPOINTS);
  MPI_Bcast(&data[0][0], NUM_FLIGHTS * NUM_WAYPOINTS, MPI_WayPoint, 0, MPI_COMM_WORLD);
	calculateDistances(data, ENDPOINTS[rank-1]-1, ENDPOINTS[rank]);
}
