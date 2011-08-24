#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "common.h"
#include "mpidelegator.h"

MPI_Datatype MPI_WayPoint, MPI_EventData;

void mpiStructInit()
{
	MPI_Datatype MPI_WayPoint_Types[4] = { MPI_FLOAT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT };
	int blocklengths_Waypoint[4] = {1,1,1,1};
	MPI_Aint displacements_Waypoint[4] = {0, sizeof(float), sizeof(float) *2, sizeof(float) * 3};

	MPI_Type_struct(4, blocklengths_Waypoint, displacements_Waypoint, MPI_WayPoint_Types, &MPI_WayPoint);
	MPI_Type_commit(&MPI_WayPoint);

	MPI_Datatype MPI_EventData_Types[8] = { MPI_FLOAT, MPI_INT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT };
	int blocklengths_EventData[8] = {1,1,1,1,1,1,1,1};
	//MPI_Aint displacements_EventData[4] = {0, sizeof(float), sizeof(float) + sizeof(int), sizeof(float) + sizeof(int) + sizeof(WayPoint)};
	MPI_Aint displacements_EventData[8] = {
		0, 
		sizeof(int), 
		sizeof(int) + sizeof(float),
		sizeof(int) + sizeof(float) * 2,
		sizeof(int) + sizeof(float) * 3,
		sizeof(int) + sizeof(float) * 4,
		sizeof(int) + sizeof(float) * 5,
		sizeof(int) + sizeof(float) * 6,
	};

	MPI_Type_struct(8, blocklengths_EventData, displacements_EventData, MPI_EventData_Types, &MPI_EventData);
	MPI_Type_commit(&MPI_EventData);
}

void hostNode(int size)
{
	WayPoint **data = readData();
	EventData **results = allocate2DEventData(NUM_FLIGHTS, NUM_WAYPOINTS); 

	if (size > 1)
	{
  	MPI_Bcast(&data[0][0], NUM_FLIGHTS * NUM_WAYPOINTS, MPI_WayPoint, 0, MPI_COMM_WORLD);
	}
	//if (OUTPUT_FLAG) { printf("Node 0 Processing %d to %d\n", ENDPOINTS[size-1]-1, 0); }
	calculateDistances(data, results, ENDPOINTS[size-1]-1, 0);
	if (size > 1)
	{
		int ranks; 
		for (ranks = 1; ranks < size; ranks++)
		{
			int i, k;
			EventData **mpiResults = allocate2DEventData(NUM_FLIGHTS, NUM_WAYPOINTS);
			MPI_Status stat;
			//printf("Waiting on Rank %d\n", ranks);
			MPI_Recv(&mpiResults[0][0], NUM_FLIGHTS * NUM_WAYPOINTS, MPI_EventData, ranks, 1, MPI_COMM_WORLD, &stat);
			printf("    Received From Rank %d: %2.2f %d %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f\n",
				ranks,
				mpiResults[0][0].distance,
				mpiResults[0][0].collision,
				mpiResults[0][0].flightOneX,
				mpiResults[0][0].flightOneY,
				mpiResults[0][0].flightOneZ,
				mpiResults[0][0].flightTwoX,
				mpiResults[0][0].flightTwoY,
				mpiResults[0][0].flightTwoZ
			);

			for (i=0; i < NUM_FLIGHTS; i++)
			{
				for (k=0; k < NUM_WAYPOINTS; k++)
				{
					if (mpiResults[i][k].collision == 1)
					{
						//printf("merging\n");
						results[i][k] = mpiResults[i][k];
					}
				}
			}
			//printf("\n");
			//free(mpiResults);
		}
	}

	/*

	printf("\n\n  ---EVENT REPORT---\n\n");
	int i, k;
	for (i=0; i < NUM_FLIGHTS; i++)
	{
		for (k=0; k < NUM_WAYPOINTS; k++)
		{
			if (results[i][k].collision == 1)
			{
				printf("Event At %d %d  :  ", i, k);
				printf(
					"%2.2f %d %2.2f %2.2f %2.2f | %2.2f %2.2f %2.2f\n",
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

	*/
}

void clientNode(int rank, int size)
{
	WayPoint **data = allocate2DWayPoints(NUM_FLIGHTS, NUM_WAYPOINTS);
	EventData **results = allocate2DEventData(NUM_FLIGHTS, NUM_WAYPOINTS); 

  MPI_Bcast(&data[0][0], NUM_FLIGHTS * NUM_WAYPOINTS, MPI_WayPoint, 0, MPI_COMM_WORLD);
	//if (OUTPUT_FLAG) { printf("Node %d Processing %d to %d\n", rank, ENDPOINTS[rank], ENDPOINTS[rank-1]); }
	calculateDistances(data, results, ENDPOINTS[rank-1]-1, ENDPOINTS[rank]);
	/*
	int i, k;
	for (i=0; i < NUM_FLIGHTS; i++)
	{
		for (k=0; k < NUM_WAYPOINTS; k++)
		{
			if (results[i][k].collision == 1)
			{
				printf("Rank %d Sending Collision Event %d with %d\n", rank, i, k);
			}
		}
	}
	*/
	results[0][0].distance = 1000 + rank;
	results[0][0].collision = 1100 + rank;
	results[0][0].flightOneX = 1200 + rank;
	results[0][0].flightOneY = 1300 + rank;
	results[0][0].flightOneZ = 1400 + rank;
	results[0][0].flightTwoX = 1500 + rank;
	results[0][0].flightTwoY = 1600 + rank;
	results[0][0].flightTwoZ = 1700 + rank;
	printf("  Rank %d Sending: %2.2f %d %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f\n",
		rank,
		results[0][0].distance,
		results[0][0].collision,
		results[0][0].flightOneX,
		results[0][0].flightOneY,
		results[0][0].flightOneZ,
		results[0][0].flightTwoX,
		results[0][0].flightTwoY,
		results[0][0].flightTwoZ
	);

	MPI_Send(results, NUM_FLIGHTS * NUM_WAYPOINTS, MPI_EventData, 0, 1, MPI_COMM_WORLD);
	free(data);
	free(results);
}
