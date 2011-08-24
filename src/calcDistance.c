#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "calcDistance.h"

void calculateDistances(WayPoint **data, EventData **results, int endPoint, int startPoint)
{
	int planeOne, planeTwo;
	for (planeOne = endPoint; planeOne > startPoint-1; --planeOne)
	{
		if (planeOne != 0)
		{
			for (planeTwo = planeOne-1; planeTwo > -1; planeTwo--)
			{
				if (CUDA_ENABLED)
				{

				}
				else
				{
					cpuDistanceCalc(planeOne, data[planeOne], planeTwo, data[planeTwo], results);
				}
			}
		}
	}
}

static void cpuDistanceCalc(int flightOneId, WayPoint *flightOne, int flightTwoId, WayPoint *flightTwo, EventData **results)
{
	int i;
	for (i = 0; i < NUM_WAYPOINTS; i++)
	{
		if (validNode(flightOne[i]) && validNode(flightTwo[i]))
		{
			EventData eventData = {
				0,
				1,
				flightOne[i].x,
				flightOne[i].y,
				flightOne[i].z,
				flightTwo[i].x,
				flightTwo[i].y,
				flightTwo[i].z
			};
			results[flightTwoId][flightOneId] = eventData;
			//printf("Event for %d %d Created\n", flightTwoId, flightOneId);
		}
	}
}

static void gpuDistanceCalc(int flightOneId, WayPoint *flightOne, int flightTwoId, WayPoint *flightTwo, EventData **results)
{

}

static int validNode(WayPoint node)
{
	return node.x != -1 && node.y != -1 && node.z != -1;
}
