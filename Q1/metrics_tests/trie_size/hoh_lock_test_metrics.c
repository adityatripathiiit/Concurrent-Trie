#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h> 

#include "./common_tests.c"


int main(int argc, char* argv[]){
    //Test begins
    
    struct timeval start, end; 

    char path[1024] = "./data/nthreads_hoh_lock.csv";

    FILE * f = fopen(path, "w");

    if(f == NULL){
        printf("Could not create the CSV file %s\n", path);
        return 0;
    }
    for(int i=1; i<=SIZE_OF_TREE; i++){
        // start timer. 
        gettimeofday(&start, NULL);

        trie = init_trie();
        
        ins_parent(i);
        find_parent(i);
        rem_parent(i);
        pref_parent(i);

        gettimeofday(&end, NULL);
        double time_taken; 

        time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
        time_taken = (time_taken + (end.tv_usec -start.tv_usec)) * 1e-6; 

        delete_trie(trie);
        fprintf(f,"%d,%f\n",i, time_taken);
        
    }
    fclose(f);
}