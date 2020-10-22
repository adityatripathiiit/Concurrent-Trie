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


void print_table(ft_table_t* table , WorkloadsT type, int cache_size,int size , int pages, float hit_rate_fifo, float hit_rate_lru, float hit_rate_approx_lru, float hit_rate_random){
	char c1[50]; //size of the number
	char c2[50]; 
	char c3[50]; 
	char c4[50]; 
	char c5[50];
	char c6[50];
	char c7[50];
    sprintf(c1, "%g",hit_rate_fifo); 
	sprintf(c2, "%g",hit_rate_lru); 
	sprintf(c3, "%g",hit_rate_approx_lru); 
	sprintf(c4, "%g",hit_rate_random); 
	sprintf(c5, "%d",cache_size); 
	sprintf(c6, "%d",size); 
	sprintf(c7, "%d",pages ); 
	if(type == LOOP){
		ft_u8write_ln(table, "LOOP",c5,c6,c7, c3, c2, c1, c4);
	}
	else if(type == RANDOM){
		ft_u8write_ln(table, "RANDOM" ,c5,c6,c7, c3, c2, c1, c4);
	}
	else if(type== LOCAL){
		ft_u8write_ln(table, "LOCAL" ,c5,c6,c7, c3, c2, c1, c4);
	}
}

int main()
{
	/* code */
	int size ; 
	int pages ; 
	WorkloadsT type_loop = LOOP; 
	WorkloadsT type_random = RANDOM; 
	WorkloadsT type_local = LOCAL ; 
	srand (time(NULL));
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
	struct workload* work_load_loop = generate_workload(type_loop, pages ,size ); 
	struct workload* work_load_random = generate_workload(type_random, pages ,size ); 
	struct workload* work_load_local = generate_workload(type_local, pages ,size ); 

	
	float hit_rate_FIFO_LOOP = policy_FIFO(work_load_loop, cache_size);
	float hit_rate_FIFO_RANDOM = policy_FIFO(work_load_random, cache_size);
	float hit_rate_FIFO_LOCAL = policy_FIFO(work_load_local, cache_size);


	float hit_rate_LRU_LOOP = policy_LRU(work_load_loop, cache_size);
	float hit_rate_LRU_RANDOM = policy_LRU(work_load_random, cache_size);
	float hit_rate_LRU_LOCAL = policy_LRU(work_load_local, cache_size);
	
	
	float hit_rate_RANDOM_LOOP = policy_RANDOM(work_load_loop, cache_size);
	float hit_rate_RANDOM_RANDOM = policy_RANDOM(work_load_random, cache_size);
	float hit_rate_RANDOM_LOCAL = policy_RANDOM(work_load_local, cache_size);
	
	float hit_rate_LRUapprox_LOOP = policy_LRUapprox(work_load_loop, cache_size);
	float hit_rate_LRUapprox_RANDOM = policy_LRUapprox(work_load_random, cache_size);
	float hit_rate_LRUapprox_LOCAL = policy_LRUapprox(work_load_local, cache_size);	



	ft_table_t *table = ft_create_table();
	ft_set_border_style(table, FT_NICE_STYLE);
	ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
	ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);
	ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
	ft_u8write_ln(table, "WorkLoad Type","Cache_size", "Size", "Pages",  "LRU_Approx Hit Rate", "LRU Hit Rate", "FIFO Hit Rate", "RANDOM Hit Rate");
	print_table(table, LOOP,cache_size, size ,pages, hit_rate_FIFO_LOOP,hit_rate_LRU_LOOP,hit_rate_LRUapprox_LOOP,hit_rate_RANDOM_LOOP); 
	print_table(table, LOCAL,cache_size, size ,pages, hit_rate_FIFO_LOCAL,hit_rate_LRU_LOCAL,hit_rate_LRUapprox_LOCAL,hit_rate_RANDOM_LOCAL); 
	print_table(table,RANDOM,cache_size, size ,pages, hit_rate_FIFO_RANDOM,hit_rate_LRU_RANDOM,hit_rate_LRUapprox_RANDOM,hit_rate_RANDOM_RANDOM); 
	printf("%s\n", (const char *)ft_to_u8string(table));
	ft_destroy_table(table);


	loop_csv(work_load_loop); 
	random_csv(work_load_random); 
	local_csv(work_load_local); 


	return 0;
}
