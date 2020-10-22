/*
INSTRUCTIONS:
This file must include only the main function
The main function should print out the various cases
and finally print out tables showing the performance
of various policies with the different workloads.
Vary the cache size.

You may include a function or 2 to ease the printing of tables.

*/

#include "definitions.h"


void print_workload(struct workload* work_load){
	for(int i=0; i<work_load->pages; i++){
		printf("%d ", work_load->work[i]); 
	}
	printf("\n"); 
}


int main()
{
	/* code */
	int size ; 
	int pages ; 
	WorkloadsT type = RANDOM; 
	int cache_size ;
	printf("Please enter the number of pages: "); 
	scanf("%d", &pages); 
	printf("\n");
	printf("Please enter the size of the workload: ");
	scanf("%d", &size ); 
	printf("\n");
	// printf("Please enter the type of the workload: ");
	// scanf("%u",&type ); 
	// printf("\n");
	printf("Please enter the cache size: ");
	scanf("%d", &cache_size); 
	printf("\n");
	struct workload* work_load = generate_workload(type, pages ,size ); 
	print_workload(work_load); 
	float hit_rate_LRU;
	float hit_rate_FIFO;
	float hit_rate_LRU_APPROX ; 
	float hit_rate_RANDOM; 
	hit_rate_FIFO = policy_FIFO(work_load, cache_size);
	hit_rate_LRU = policy_LRU(work_load,cache_size);
	hit_rate_LRU_APPROX = policy_LRUapprox(work_load, cache_size); 
	hit_rate_RANDOM = policy_RANDOM(work_load, cache_size); 

	return 0;
}
