#ifndef CALCDISTANCE_H
	#define CALCDISTANCE_H

	void calculateDistances(WayPoint **data, EventData **results, int endPoint, int startPoint);
	static void cpuDistanceCalc(int flightOneId, WayPoint *flightOne, int flightTwoId, WayPoint *flightTwo, EventData **results);
	static void gpuDistanceCalc(int flightOneId, WayPoint *flightOne, int flightTwoId, WayPoint *flightTwo, EventData **results);
	static int validNode(WayPoint node);

#endif
