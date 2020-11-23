/* INSTRUCTIONS:

This file will contain all functions related with various policies.
Each policy must return the hit rate

*/

#include "definitions.h"

// FIFO policy
float policy_FIFO(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	int size = w->size ;
	int pages = w->pages ;
	// Using an array based queue data structure 
	// Insert at the ith index & lookup : O(1) Time complexity
	int queue[cache_size]; 
	int hits = 0 ;
	// a cicular pointer for mimicing the pop from FIFQ queue 
	// Represents head of the queue
	int k = 0; 
	//Initializing the pages queue to with -1
	for(int i=0;i<cache_size;i++){
		queue[i] = -1; 
	}
	// Total Time complexity: O(pages*cache_size)
	for(int i=0; i<size; i++){
		int check = w->work[i]; // Current page 
		int found_flag = 0 ;
		// For every page, takes O(cache_size) time to find it in the queue
		for(int j=0; j<cache_size; j++){
			if(queue[j] == check){
				hits++;
				found_flag = 1;
				break ;
			}
		}
		// If page is not found, pop from the head and add the page
		// Move the pointer circularly
		// O(1) operation
		if(found_flag ==0){
			queue[k] = check;
			k = (k+1)%cache_size;
		}
	}
	hit_rate = (100*(float)hits)/((float)size); 
	/* fill this */
	return hit_rate;
}

// LRU(Least Recently Used) Policy 
float policy_LRU(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	int size = w->size ;
	int pages = w->pages ;
	int hits = 0 ;
	int least_rec_used ;
	// Array based 2d queue
	// one to maintain the page value
	// The second one is used to maintain the priority. 

	int cache[cache_size][2]; 
	// Initialisation to -1
	for(int i=0; i<cache_size; i++){
		cache[i][0] = -1 ;
		cache[i][1] = -1 ;
	}
	// Overall complexity: O(pages*cache_size)
	for(int i=0; i<size; i++){
		
		int flag1 = 0;
		int flag2 = 0;
		int check = w->work[i]; // current page 
		int index = 0;
		// O(cache_size) to find and replace if necessary
		for(int j=0; j<cache_size; j++){
			if(check == cache[j][0]){
				flag1 = 1;
				cache[j][1] = i ;  // Updating the priority of the page 
				hits++;
				break; 
			}
		}
		if(flag1 ==0){
			least_rec_used = i ; 
			for(int j=0; j<cache_size; j++){
				if(cache[j][1] < least_rec_used ){ // The value with the least priority is removed
					least_rec_used = cache[j][1]; 
					index = j ;
					flag2 = 1;
				}
			}
			// The position with least priority is filled with new value
			if(flag2 == 1){
				cache[index][0] = check ;
				cache[index][1] = i;
			}
		}
	}

	hit_rate = (100*(float)hits)/((float)size); 
	/* fill this */
	return hit_rate;
}

// Arrox_LRU/ Clock Hand mechanism/ Second chance algorithm
float policy_LRUapprox(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	int size = w->size ;
	int pages = w->pages ;
	int hits = 0 ;
	// Array based 2d queue
	// one to maintain the page value
	// The second one is used to maintain the second chance flag
	int cache[cache_size][2]; 
	// Clock hand, circular pointer
	int hand = 0 ;
	// Initialisation to -1 and 0
	for(int i=0; i<cache_size; i++){
		cache[i][0] = -1 ;
		cache[i][1] = 0;
	}
	// Overall complexity: O(pages*cache_size)
	for(int i=0; i<size; i++){
		int check = w->work[i]; //current page
		int found = 0 ;
		int inserted = 0 ; 
		// O(cache_size) to find and replace if necessary
		for(int j=0; j<cache_size; j++){
			if(check == cache[j][0]){
				found = 1;
				cache[j][1] = 1; // Update the second chance flag, if page found
				hits++;
				break; 
			}
		}
		if(found ==0){
			do{
				// If there is no second chance available, replace the page
				if(cache[hand][1] == 0){
					cache[hand][0] = check ;
					cache[hand][1] = 0; 
					inserted = 1;
				}
				else{
					cache[hand][1] = 0; // Every page through which hand passes loses its second chance
				}
				hand = (hand+1)%cache_size; // move the clock hand circularly
			}while(inserted != 1 ); // find the page which has no second chance

		}
	}

	// printf("LRUApprox hits: %d\n",hits); 
	// printf("LRUArrox miss: %d\n",pages-hits); 
	hit_rate = (100*(float)hits)/((float)size); 
	/* fill this */
	return hit_rate;
}

// Random Policy
float policy_RANDOM(struct workload * w, int cache_size)
{	
	float hit_rate = 0;
	int size = w->size ;
	int pages = w->pages ;
	// Using an array based queue data structure 
	// Insert at the ith index & lookup : O(1) Time complexity
	int queue[cache_size]; 
	int hits = 0 ;
	// pointer for random page elemination 
	int k = rand()%cache_size; 
	//Initializing the pages queue to with -1
	for(int i=0;i<cache_size;i++){
		queue[i] = -1; 
	}
	// Total Time complexity: O(pages*cache_size)
	for(int i=0; i<size; i++){
		int check = w->work[i]; // Current page 
		int found_flag = 0 ;
		// For every page, takes O(cache_size) time to find it in the queue
		for(int j=0; j<cache_size; j++){
			if(queue[j] == check){
				hits++;
				found_flag = 1;
				break ;
			}
		}
		// If page is not found, pop from a random position and add the page
		// Assign the pointer a random value
		// O(1) operation
		if(found_flag ==0){
			queue[k] = check;
			k = rand()%cache_size;
		}
	}
	hit_rate = (100*(float)hits)/((float)size); 
	
	/* fill this */
	return hit_rate;
}
