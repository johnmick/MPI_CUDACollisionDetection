#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "utilities.h"

WayPoint **allocate2DWayPoints(int numFlights, int numWayPoints) 
{
	int i;
	WayPoint *spaceReserved = (WayPoint *)malloc(numFlights * numWayPoints * sizeof(WayPoint));
	WayPoint **data = (WayPoint **)malloc(numFlights * sizeof(WayPoint *));
	for (i = 0; i < numFlights; i++)
	{
		data[i] = spaceReserved + (i * numWayPoints);
	}
	return data;
}
