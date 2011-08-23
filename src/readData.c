#include <string.h>
#include <stdio.h>
#include "common.h"
#include "utilities.h"
#include "readData.h"

WayPoint **readData(char* folderPath, char* fileName, int numFlights, int numWayPoints)
{
	char fileNameAndPathFormat[255];
	char fileNameAndPath[255];
	int i, k;
	FILE *inputFile;

	strcat(fileNameAndPathFormat, FILE_DIR_PATH);
	strcat(fileNameAndPathFormat, FILE_INPUT_NAME);
	WayPoint **data = allocate2DWayPoints(numFlights, numWayPoints);

	if (OUTPUT_FLAG) { printf("Reading %d Flights with %d WayPoints Each From \"%s\": ", numFlights, numWayPoints, fileNameAndPathFormat); }

	for (i =0; i < numFlights; i++)
	{
		sprintf(fileNameAndPath, fileNameAndPathFormat, i);
		inputFile = fopen(fileNameAndPath, "r");
		for (k = 0; k < numWayPoints; k++)
		{
			fscanf(
				inputFile, 
				"%f %f %f %f\n", 
				&data[i][k].t,
				&data[i][k].x,
				&data[i][k].y,
				&data[i][k].z
			);
		}
		fclose(inputFile);
	}

	if (OUTPUT_FLAG) { printf("Done\n"); }
	return data;
}
