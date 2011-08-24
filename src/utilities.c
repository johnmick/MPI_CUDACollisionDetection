#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "utilities.h"

WayPoint **allocate2DWayPoints(int numFlights, int numWayPoints) 
{
	int i;
	WayPoint *spaceReserved = (WayPoint *)malloc(numFlights * numWayPoints * sizeof(WayPoint));
	//WayPoint **data = (WayPoint **)malloc(numFlights * sizeof(WayPoint *));
	WayPoint **data = (WayPoint **)calloc(numFlights, sizeof(WayPoint *));
	for (i = 0; i < numFlights; i++)
	{
		data[i] = spaceReserved + (i * numWayPoints);
	}
	return data;
}

EventData **allocate2DEventData(int numFlights, int numEventDatas) 
{
	int i;
	EventData *spaceReserved = (EventData *)malloc(numFlights * numEventDatas * sizeof(EventData));
	//EventData **data = (EventData **)malloc(numFlights * sizeof(EventData *));
	EventData **data = (EventData **)calloc(numFlights, sizeof(EventData *));
	for (i = 0; i < numFlights; i++)
	{
		data[i] = spaceReserved + (i * numEventDatas);
	}
	return data;
}

EventData *allocateEventData(int numFlights, int numEventDatas) 
{
	//EventData *data = (EventData *)malloc(numFlights * numEventDatas * sizeof(EventData));
	EventData *data = (EventData *)calloc(numFlights, numEventDatas * sizeof(EventData));
	return data;
}
