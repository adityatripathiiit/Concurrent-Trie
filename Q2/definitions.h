/* 
INSTRUCTIONS:

All enum, structs and functions to be declared here.
If a definition is already declared, do not change it.
You may add extra definitions as required.

*/

#ifndef __DEF_H__
#define __DEF_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "fort.h"
//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL ENUMERATORS HERE */

// LOOP - Workload type for Loop
// RANDOM - Workload type for RANDOM
// LOCAL - 80-20 workload type

typedef enum enum_workloads{LOOP, RANDOM, LOCAL}WorkloadsT;

//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL STRUCTS HERE */

struct workload
{
	// Defines type of workload
	WorkloadsT type;
	// The number of pages in workload
	int pages;
	// The total size of the workload
	int size;
	// The final work to run the policy
	int * work; // https://www.geeksforgeeks.org/pointer-array-array-pointer/
};

//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL FUNCTION DEFINITIONS HERE */

struct workload * generate_workload(WorkloadsT type, int pages, int size);
struct workload * generate_random(struct workload * w);
struct workload * generate_loop(struct workload * w);
struct workload * generate_local(struct workload * w);

float policy_FIFO(struct workload * w, int cache_size);
float policy_LRU(struct workload * w, int cache_size);
float policy_RANDOM(struct workload * w, int cache_size);
float policy_LRUapprox(struct workload * w, int cache_size);

#endif /* __DEF_H__ */
