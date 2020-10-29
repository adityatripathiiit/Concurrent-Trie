/*
INSTRUCTIONS:

This file will contain all the functions related to the generation of workload

*/

#include "definitions.h"

// Note: The work must be generated for the returned pointer
struct workload * generate_workload(WorkloadsT type, int pages, int size)
{
    struct workload* work_load =  (struct workload*)malloc(sizeof(struct workload))  ; 
    work_load->type = type; 
    work_load-> pages = pages;
    work_load->size = size ;
    // Checking the type of workload to generate 
    if(type == LOCAL){
        work_load = generate_local(work_load);
    }
    else if(type == RANDOM){
        work_load = generate_random(work_load); 
    }
    else if(type == LOOP){
        work_load = generate_loop(work_load); 
    }
    return work_load ;
}

struct workload * generate_random(struct workload * w)
{
    int size = w->size ;
    int pages = w->pages ;
    int *work_array =  malloc(pages*sizeof(int)); 
    // Randomly generating a number between 0 and pages
    for(int i=0 ; i<pages; i++){
        work_array[i] = rand()%size ;
    }
    w->work = work_array; 
    return w;
}

struct workload * generate_loop(struct workload * w)
{

    int size = w->size ;
    int pages = w->pages ;
    int *work_array =  malloc(pages*sizeof(int)); 
    // generating loop style work load
    for(int i=0 ; i< floor(pages/size); i++){
        for(int j=0; j<size; j++){
            work_array[i*size+j] = j ;
        }
    }
    // If the pages are not perfectly divisible by number of unique pages
    // put the rest in the workload
    if(pages%size != 0){
        for(int i=0; i<pages%size; i++){
            work_array[(int)floor(pages/size)*size+i] = i; 
        }
    }
    w->work = work_array; 
    return w;
}

struct workload * generate_local(struct workload * w)
{
    int size = w->size ;
    int pages = w->pages ;
    int *work_array =  malloc(pages*sizeof(int)); 

    int major = floor(pages*0.8);
    int minor = floor(pages*0.2); 
    // 80% pages from 20% unique pages
    for(int i=0 ; i<major; i++){
        work_array[i] = rand()%((int)floor(size*0.2));
    }
    // 20% pages from rest 80% unique pages
    for(int i=0; i<minor; i++){
        work_array[i+major] = rand()%((int)floor(size*0.8))+ (int)floor(size*0.2);
    }
    int left = pages -( major+minor ); 
    for(int i=0; i<left; i++){
        work_array[major+minor+i] = i ; 
    }

    // Randomising the ordering of the 80% and 20 % unqiue pages 
    // As they were eariler inserted sequentially.
    for(int i=0; i<pages; i++){
        int rand_index = rand()%pages;
        int temp = work_array[i];
        work_array[i] = work_array[rand_index]; 
        work_array[rand_index] = temp; 
    }

    w->work = work_array; 
    return w;
}
