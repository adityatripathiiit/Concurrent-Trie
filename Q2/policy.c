/* INSTRUCTIONS:

This file will contain all functions related with various policies.
Each policy must return the hit rate

*/

#include "definitions.h"

float policy_FIFO(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	int size = w->size ;
	int pages = w->pages ;
	int queue[cache_size]; 
	int hits = 0 ;
	int k = 0; 
	for(int i=0;i<cache_size;i++){
		queue[i] = -1; 
	}
	for(int i=0; i<pages; i++){
		int check = w->work[i];
		int found_flag = 0 ;
		for(int j=0; j<cache_size; j++){
			if(queue[j] == check){
				hits++;
				found_flag = 1;
				break ;
			}
		}
		if(found_flag ==0){
			queue[k] = check;
			k = (k+1)%cache_size;
		}
		// for(int j = 0; j<cache_size; j++){
		// 	printf("%d ", queue[j]); 
		// }
		// printf("\n"); 
	}
	hit_rate = (100*(float)hits)/((float)pages); 
	// printf("FIFO hits: %d\n",hits); 
	// printf("FIFO misses: %d\n", pages-hits); 
	/* fill this */
	return hit_rate;
}

float policy_LRU(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	int size = w->size ;
	int pages = w->pages ;
	int hits = 0 ;
	int least_rec_used ;
	int cache[pages][2]; 
	for(int i=0; i<pages; i++){
		cache[i][0] = -1 ;
		cache[i][1] = -1 ;
	}
	for(int i=0; i<pages; i++){
		
		int flag1 = 0;
		int flag2 = 0 ;
		int check = w->work[i]; 
		int index = 0;
		for(int j=0; j<cache_size; j++){
			if(check == cache[j][0]){
				flag1 = 1;
				cache[j][1] = i ; 
				hits++;
				break; 
			}
		}
		if(flag1 ==0){
			least_rec_used = i ; 
			for(int j=0; j<cache_size; j++){
				if(cache[j][1] < least_rec_used ){
					least_rec_used = cache[j][1]; 
					index = j ;
					flag2 = 1;
				}
			}
			if(flag2 == 1){
				cache[index][0] = check ;
				cache[index][1] = i;
			}
		}
	}

	// printf("LRU hits: %d\n",hits); 
	// printf("LRU miss: %d\n",pages-hits); 
	hit_rate = (100*(float)hits)/((float)pages); 
	/* fill this */
	return hit_rate;
}

float policy_LRUapprox(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	int size = w->size ;
	int pages = w->pages ;
	int hits = 0 ;
	int cache[pages][2]; 
	int hand = 0 ;
	for(int i=0; i<pages; i++){
		cache[i][0] = -1 ;
		cache[i][1] = 0;
	}
	for(int i=0; i<pages; i++){
		int check = w->work[i]; 
		int found = 0 ;
		int inserted = 0 ; 
		for(int j=0; j<cache_size; j++){
			if(check == cache[j][0]){
				found = 1;
				cache[j][1] = 1 ; 
				hits++;
				break; 
			}
		}
		if(found ==0){
			do{
				if(cache[hand][1] == 0){
					cache[hand][0] = check ;
					cache[hand][1] = 0; 
					inserted = 1;
				}
				else{
					cache[hand][1] = 0; 
				}
				hand = (hand+1)%cache_size;
			}while(inserted != 1 );

		}
	}

	// printf("LRUApprox hits: %d\n",hits); 
	// printf("LRUArrox miss: %d\n",pages-hits); 
	hit_rate = (100*(float)hits)/((float)pages); 
	/* fill this */
	return hit_rate;
}

float policy_RANDOM(struct workload * w, int cache_size)
{	
	float hit_rate = 0;
	int size = w->size ;
	int pages = w->pages ;
	int queue[cache_size]; 
	int hits = 0 ;
	int k = rand()%cache_size; 
	for(int i=0;i<cache_size;i++){
		queue[i] = -1; 
	}
	for(int i=0; i<pages; i++){
		int check = w->work[i];
		int found_flag = 0 ;
		for(int j=0; j<cache_size; j++){
			if(queue[j] == check){
				hits++;
				found_flag = 1;
				break ;
			}
		}
		if(found_flag ==0){
			queue[k] = check;
			k = rand()%cache_size;
		}
	}
	hit_rate = (100*(float)hits)/((float)pages); 
	// printf("Random hits: %d\n",hits); 
	// printf("Random misses: %d\n", pages-hits); 
	/* fill this */
	return hit_rate;
}
