// Function to help generation of CSV files for different workloads
// and different policies
#include "definitions.h"

void create_csv(char* policy_type, char* type, struct workload* work_load ){
    // setting the path name according to the policy type and work load type
    char path[1024] = ""; 
    strcat(path , "./data/");
    strcat(path,type);
    strcat(path,"_");
    strcat(path,policy_type);
    strcat(path,".csv"); 
    // Opening the CSV file for writing data
    FILE * f = fopen(path, "w"); 
    if(f == NULL){
        printf("Could not create the CSV file %s\n", type);
        return ;
    }
    int pages = work_load->pages ;
    int cache_size;
    // Depending on the policy type, enter data by running workload on the policy
    if(policy_type == "FIFO"){
        for(cache_size = 1;cache_size<=work_load->size; cache_size ++ ){
            fprintf(f,"%d,%f\n",cache_size, policy_FIFO(work_load,cache_size));
        }
    }
    else if(policy_type == "LRU"){
        for(cache_size = 1;cache_size<=work_load->size; cache_size ++ ){
            fprintf(f,"%d,%f\n",cache_size, policy_LRU(work_load,cache_size));
        }
    }
    else if(policy_type == "LRUAppr"){
        for(cache_size = 1;cache_size<=work_load->size; cache_size ++ ){
            fprintf(f,"%d,%f\n",cache_size, policy_LRUapprox(work_load,cache_size));
        }
    }
    else if(policy_type == "RANDOM"){
        for(cache_size = 1;cache_size<=work_load->size; cache_size ++ ){
            fprintf(f,"%d,%f\n",cache_size, policy_RANDOM(work_load,cache_size));
        }
    }
    fclose(f); 
}

// Functions for creating all combinations of workload
// and policy
void loop_csv(struct workload* work_load){
    create_csv("FIFO","LOOP",  work_load); 
    create_csv("LRU", "LOOP",work_load); 
    create_csv("LRUAppr","LOOP", work_load); 
    create_csv("RANDOM", "LOOP",work_load); 
}

void random_csv(struct workload* work_load){
    create_csv("FIFO","RANDOM", work_load); 
    create_csv("LRU", "RANDOM", work_load); 
    create_csv("LRUAppr", "RANDOM", work_load); 
    create_csv("RANDOM", "RANDOM", work_load); 
}

void local_csv(struct workload* work_load){
    create_csv("FIFO","LOCAL", work_load); 
    create_csv("LRU","LOCAL", work_load); 
    create_csv("LRUAppr", "LOCAL",work_load); 
    create_csv("RANDOM", "LOCAL",work_load); 
}