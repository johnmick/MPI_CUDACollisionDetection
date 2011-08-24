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
	WayPoint **data = readData();
	EventData **results = allocate2DEventData(NUM_FLIGHTS, NUM_WAYPOINTS); 

	if (size > 1)
	{
  	MPI_Bcast(&data[0][0], NUM_FLIGHTS * NUM_WAYPOINTS, MPI_WayPoint, 0, MPI_COMM_WORLD);
	}
	calculateDistances(data, results, ENDPOINTS[size-1]-1, 0);
	if (size > 1)
	{
		int ranks; 
		for (ranks = 1; ranks < size; ranks++)
		{
			int i, k;
			EventData **mpiResults = allocate2DEventData(NUM_FLIGHTS, NUM_WAYPOINTS);
			MPI_Status stat;
			MPI_Recv(&mpiResults[0][0], NUM_FLIGHTS * NUM_WAYPOINTS, MPI_WayPoint, ranks, 1, MPI_COMM_WORLD, &stat);
			printf("Rank %d Reported Preparing Merge: %2f %d %2f %2f %2f | %2f %2f %2f\n",
				ranks,
				mpiResults[0][0].distance,
				mpiResults[0][0].collision,
				mpiResults[0][0].flightOne.x,
				mpiResults[0][0].flightOne.y,
				mpiResults[0][0].flightOne.z,
				mpiResults[0][0].flightTwo.x,
				mpiResults[0][0].flightTwo.y,
				mpiResults[0][0].flightTwo.z
			);
			for (i=0; i < NUM_FLIGHTS; i++)
			{
				for (k=0; k < NUM_WAYPOINTS; k++)
				{
					if (mpiResults[i][k].collision == 1)
					{
						printf("merging\n");
						results[i][k] = mpiResults[i][k];
					}
				}
			}
		}
	}

	printf("\n\n  ---EVENT REPORT---\n\n");

	int i, k;
	for (i=0; i < NUM_FLIGHTS; i++)
	{
		for (k=0; k < NUM_WAYPOINTS; k++)
		{
			if (results[i][k].collision == 1)
			{
				printf("Event At %d %d\n", i, k);
				printf(
					"%2f %d %2f %2f %2f | %2f %2f %2f\n",
					results[i][k].distance,
					results[i][k].collision,
					(results[i][k].flightOne).x,
					(results[i][k].flightOne).y,
					(results[i][k].flightOne).z,
					(results[i][k].flightTwo).x,
					(results[i][k].flightTwo).y,
					(results[i][k].flightTwo).z
				);
			}

		}
	}
}

void clientNode(int rank, int size)
{
	WayPoint **data = allocate2DWayPoints(NUM_FLIGHTS, NUM_WAYPOINTS);
	EventData **results = allocate2DEventData(NUM_FLIGHTS, NUM_WAYPOINTS); 

  MPI_Bcast(&data[0][0], NUM_FLIGHTS * NUM_WAYPOINTS, MPI_WayPoint, 0, MPI_COMM_WORLD);
	if (OUTPUT_FLAG) { printf("Node %d Processing %d to %d\n", rank, ENDPOINTS[rank], ENDPOINTS[rank-1]); }
	calculateDistances(data, results, ENDPOINTS[rank-1]-1, ENDPOINTS[rank]);
	MPI_Send(results, NUM_FLIGHTS * NUM_WAYPOINTS, MPI_WayPoint, 0, 1, MPI_COMM_WORLD);
}
