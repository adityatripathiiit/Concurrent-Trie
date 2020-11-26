/*
INSTRUCTIONS:

This file will contain all the functions related to the generation of workload

*/

#include "definitions.h"


/*
    struct workload * generate_workload(WorkloadsT type, int pages, int size): 
    Generates the workload for the given input paramaters.     
    Return the generated workload pointer
    Note: The work must be generated for the returned pointer
*/ 
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

/*
    struct workload * generate_random(struct workload * w)
    Sets the workload array for random workload, depending on the parameter of workload
    Assigns the array to the input workload 
    Return the generated workload pointer
    Note: The work must be generated for the returned pointer
*/ 
struct workload * generate_random(struct workload * w)
{
    int size = w->size ;
    int pages = w->pages ;
    int *work_array =  malloc(size*sizeof(int)); 
    // Randomly generating a number between 0 and pages
    for(int i=0 ; i<size; i++){
        work_array[i] = rand()%pages ;
    }
    w->work = work_array; 
    return w;
}


/*
    struct workload * generate_loop(struct workload * w)
    Sets the workload array for looping workload, depending on the parameter of workload
    Assigns the array to the input workload 
    Return the generated workload pointer
    Note: The work must be generated for the returned pointer
*/ 

struct workload * generate_loop(struct workload * w)
{

    int size = w->size ;
    int pages = w->pages ;
    int *work_array =  malloc(size*sizeof(int)); 
    // generating loop style work load
    for(int i=0 ; i< floor(size/pages); i++){
        for(int j=0; j<pages; j++){
            work_array[i*pages+j] = j ;
        }
    }
    // If the pages are not perfectly divisible by number of unique pages
    // put the rest in the workload
    if(size%pages != 0){
        for(int i=0; i<size%pages; i++){
            work_array[(int)floor(size/pages)*pages+i] = i; 
        }
    }
    w->work = work_array; 
    return w;
}

/*
    struct workload * generate_local(struct workload * w)
    Sets the workload array for local(80-20) workload, depending on the parameter of workload
    Assigns the array to the input workload 
    Return the generated workload pointer
    Note: The work must be generated for the returned pointer
*/ 
struct workload * generate_local(struct workload * w)
{
    int size = w->size ;
    int pages = w->pages ;
    int *work_array =  malloc(size*sizeof(int)); 

    int major = floor(size*0.8);
    int minor = floor(size*0.2); 
    // 80% pages from 20% unique pages
    for(int i=0 ; i<major; i++){
        work_array[i] = rand()%((int)floor(pages*0.2));
    }
    // 20% pages from rest 80% unique pages
    for(int i=0; i<minor; i++){
        work_array[i+major] = rand()%((int)floor(pages*0.8))+ (int)floor(pages*0.2);
    }
    int left = size -( major+minor ); 
    for(int i=0; i<left; i++){
        work_array[major+minor+i] = i ; 
    }

    // Randomising the ordering of the 80% and 20 % unqiue pages 
    // As they were eariler inserted sequentially.
    for(int i=0; i<size; i++){
        int rand_index = rand()%size;
        int temp = work_array[i];
        work_array[i] = work_array[rand_index]; 
        work_array[rand_index] = temp; 
    }

    w->work = work_array; 
    return w;
}
