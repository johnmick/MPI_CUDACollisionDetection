#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "common.h"

/* This method determines how many calculations each processor should handle
   and then algebraically determines where the next cut-off value is.
   The algorithm is based on the formula n(n+1)/2 - x(x+1)/2 = number per processor.
   We round to x^2/2 since we assume 1 will be irrelevant in comparison.
*/
void loadBalance(int numProcessors, int numArrayElements, int* upperVals) {
	int eachProcLoad;

	// Calculate the total number of comparisons each process should handle
	eachProcLoad = numArrayElements * (numArrayElements + 1) / (2 * numProcessors);

	// The first upper limit will simply be the size of the passed array
	upperVals[0] = numArrayElements;

	int i;
	for (i = 1; i <= numProcessors; i++) {
		// Calculate the next upper limit
		upperVals[i] = sqrt(upperVals[i-1] * (upperVals[i-1]+1) - 2 * eachProcLoad);
	}
}




/* This method determines how many calculations each processor should handle
   and then adds each decremented value until the number of calculations for the
   process is reached.
*/
void loadBalance2(int numProcessors, int numArrayElements, int* upperVals) {

	int eachProcLoad;
	eachProcLoad = numArrayElements * (numArrayElements + 1) / (2 * numProcessors);

	upperVals[0] = numArrayElements;

	int i, n = 1, total = 0;
	for (i = numArrayElements-1; i > 0; i--) {
		total += i;
		if (total >= eachProcLoad) {
			upperVals[n++] = i;
			total = 0;
		}
	}
}
