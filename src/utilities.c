#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "utilities.h"

WayPoint **allocate2DWayPoints(int numFlights, int numWayPoints) 
{
	//if (OUTPUT_FLAG) { printf("Allocating a 2D Array with enough space for %d Arrays Containing %d WayPoints each...", numFlights, numWayPoints); }
	int i;
	WayPoint *spaceReserved = (WayPoint *)malloc(numFlights * numWayPoints * sizeof(WayPoint));
	WayPoint **data = (WayPoint **)malloc(numFlights * sizeof(WayPoint *));
	for (i = 0; i < numFlights; i++)
	{
		data[i] = spaceReserved + (i * numWayPoints);
	}

	//if (OUTPUT_FLAG) { printf("Done\n"); }

	return data;
}
