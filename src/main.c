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

	setDefaultGlobals();
	checkEnvironmentArguments(argc, argv);
	ENDPOINTS = malloc(size * sizeof(int));
	loadBalance(size, NUM_FLIGHTS, ENDPOINTS);

	if (rank == 0)
	{
		hostNode(size);
	}
	else if (size > 1)
	{
		clientNode(rank, size);
	}

	MPI_Finalize();
	return 0;
}

void setDefaultGlobals()
{
	OUTPUT_FLAG = 0;
	CUDA_ENABLED = 0;
	NUM_FLIGHTS = 100;
	NUM_WAYPOINTS = 10001;
	strcpy(FILE_INPUT_NAME, "outfile%d.txt");
	strcpy(FILE_DIR_PATH, "flights/");
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
					MPI_Finalize();
					exit(0);
				}
				break;
			case 'w':
				NUM_WAYPOINTS = atoi(optarg);
				if (NUM_WAYPOINTS < 1)
				{
					printf("Invalid Number of Waypoints - Must Specify At Least 1\n");
					MPI_Finalize();
					exit(0);
				}
				break;
			case 'd':
				strcpy(FILE_DIR_PATH, optarg);
				break;
			case 'n':
				strcpy(FILE_INPUT_NAME, optarg);
				break;
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
	printf("  -d    Specify Alternative Flights Data Directory          (DEFAULT \"flights/\")\n");
	printf("  -n    Specify Alternative Flights File Name               (DEFAULT \"outfile%%d.txt/\")\n");
	printf("  -f    Specify Alternative Number of Flights               (DEFAULT: 10)\n");
	printf("  -w    Specify Alternative Number of WayPoints per Vehicle (DEFAULT: 10001)\n");
	//printf("  -m    Enable MPI Load Balancing\n");
	//printf("  -c    Enable CUDA Based Distance Calculations\n");
	//printf("  -t    Enabled Metric Measurements\n");
	MPI_Finalize();
	exit(0);
}
