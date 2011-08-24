#ifndef COMMON_H
	#define COMMON_H

	typedef struct {
		float x; // Horizontal Axis
		float y; // Horizontal Axis
		float z; // Vertical (Elevation) Axis
		float t; // Time in No Defined Metric
	} WayPoint;

	typedef struct {
		float distance;
		int collision;
		float flightOneX;
		float flightOneY;
		float flightOneZ;
		float flightTwoX;
		float flightTwoY;
		float flightTwoZ;
	} EventData;

	//typedef struct WayPoint* Trajectory;

	int OUTPUT_FLAG;
	int NUM_FLIGHTS;
	int NUM_WAYPOINTS;
	int *ENDPOINTS;

	int CUDA_ENABLED;

	char FILE_DIR_PATH[255];
	char FILE_INPUT_NAME[255];

	static const char *optString = "w:f:e?d:n:";
#endif
