#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h> 

#define _S_LOCK_TRIE
#define _NO_HOH_LOCK_TRIE
#include "./common_tests.c"


int main(int argc, char* argv[]){
    //Test begins
    
    // char key[MAX_TREE_SIZE +5];
    // for(int i=0; i<NUMBER_OF_CONCURRENT_THREADS; i++){
    //     narr[i] = i+1;
    // }

    struct timeval start, end; 

    char path[1024] = "./data/nthreads_s_lock.csv";

    FILE * f = fopen(path, "w");

    if(f == NULL){
        printf("Could not create the CSV file %s\n", path);
        return 0;
    }
    int i = 1;
    // for(int i=1; i<=SIZE_OF_TREE; i++){
        // start timer. 
        // printf("this is i: %d \n", i);
        gettimeofday(&start, NULL);

        trie = init_trie();
        
        ins_parent(i);
        find_parent(i);
        rem_parent(i);
        // pref_parent(i);

        gettimeofday(&end, NULL);
        for(int i=0; i<26; i++){
            if(trie->head->children[i]) printf("%c\n",(i+'a'));
        }
        // long long elapsed = (end.tv_sec-start.tv_sec)*1000000LL + end.tv_usec-end.tv_usec;
        double time_taken; 
  
        time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
        time_taken = (time_taken + (end.tv_usec -start.tv_usec)) * 1e-6; 
        // delete_trie(trie);
        fprintf(f,"%d,%f\n",i, time_taken);
        
    // }
    fclose(f);
}