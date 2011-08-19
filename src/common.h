#ifndef COMMON_H
	#define COMMON_H

	typedef struct {
		float x;
		float y;
		float z;
		float t;
	} WayPoint;

	typedef struct {
		float distance;
		int collision;
	} EventData;

	typedef struct WayPoint* Trajectory;

	int OUTPUT_FLAG;
	int NUM_FLIGHTS;
	int NUM_WAYPOINTS;
	int *ENDPOINTS;

	char FULL_FILE_PATH[255];

	static const char *optString = "w:f:e?";
#endif
