#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include "common.h"
#include "main.h"


int main (int argc, char **argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	mpiStructInit();

	setDefaultGlobals(size);
	checkEnvironmentArguments(argc, argv);

	if (rank == 0)
	{
		hostNode(size);
	}
	else
	{
		clientNode(rank, size);
	}

	MPI_Finalize();
	return 0;
}

void setDefaultGlobals(int size)
{
	OUTPUT_FLAG = 0;
	NUM_FLIGHTS = 100;
	NUM_WAYPOINTS = 10001;
	ENDPOINTS = malloc(size * sizeof(int));
	strcpy(FULL_FILE_PATH, "flights/outfile%d.txt");
	loadBalance(size, NUM_FLIGHTS, ENDPOINTS);
}

void checkEnvironmentArguments(int argc, char **argv)
{
	int opt = 0;
	opt = getopt(argc, argv, optString);
	while (opt != -1)
	{
		switch (opt)
		{
			case 'f':
				NUM_FLIGHTS = atoi(optarg);
				if (NUM_FLIGHTS < 2)
				{
					printf("Invalid Number of Flights - Must Specify At Least 2\n");
					exit(0);
				}
				break;
			case 'w':
				NUM_WAYPOINTS = atoi(optarg);
				if (NUM_WAYPOINTS < 1)
				{
					printf("Invalid Number of Waypoints - Must Specify At Least 1\n");
					exit(0);
				}
			case 'e':
				OUTPUT_FLAG = 1;
				break;
			case '?':
				displayHelp();
				break;
			default:
				displayHelp();
				break;
		}
		opt = getopt(argc, argv, optString);
	}
}

void displayHelp()
{
	printf("FLAGS:\n");
	printf("  -e    Shows Program Output\n");
	// TO DO
	//printf("  -m    Enabled Metric Measurements\n");
	//printf("  -fd   Specify Alternative Flights Data Directory (DEFAULT \"flights/\")\n");
	//printf("  -fn   Specify Alternative Flights File Name (DEFAULT \"outfile%%d.txt/\")\n");
	printf("  -f    Specify Alternative Number of Flights (DEFAULT: 10)\n");
	printf("  -w    Specify Alternative Number of WayPoints per Vehicle (DEFAULT: 10001)\n");
	exit(0);
}
