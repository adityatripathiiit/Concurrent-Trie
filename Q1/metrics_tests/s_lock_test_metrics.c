#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h> 

#define _S_LOCK_TRIE
#define _NO_HOH_LOCK_TRIE
#include "../trie.h"

#define NUMBER_OF_CONCURRENT_THREADS 100 
#define MAX_TREE_SIZE 10
trie_t trie; 
int narr[NUMBER_OF_CONCURRENT_THREADS];


void* insert_task(void *tnum){
    int t = *((int *) tnum);
    char key[15];
    int val;
    char ins_file_name[100];
    sprintf(ins_file_name, "./workload/initial/%d.txt",t);
    FILE* ins_file = fopen(ins_file_name,"r");
    while (1)
    {
        fscanf(ins_file,"%s",key);
        if (key[0]=='-') break;
        fscanf(ins_file,"%d",&val);
        insert(trie,key,val);
    }
    fclose(ins_file);
    return NULL;
}

void* find_task(void *tnum){
    int t = *((int *) tnum);
    char key[15];
    char find_test_name[100];
    sprintf(find_test_name,"./workload/find/%d.txt",t);
    FILE* find_test = fopen(find_test_name,"r");
    while (1)
    {
        int f_ret = -101;
        fscanf(find_test,"%s",key);
        if (key[0]=='-') break;
        find(trie,key,&f_ret);
    }
    fclose(find_test);
    return NULL;
}

void *rem_task(void *tnum){
    int t = *((int *) tnum);
    char key[15];
    char rem_file_name[100];
    sprintf(rem_file_name,"./workload/rem/%d.txt",t);
    FILE* rem_file = fopen(rem_file_name,"r");
    while (1)
    {
        fscanf(rem_file,"%s",key);
        if (key[0]=='-') break;
        delete_kv(trie,key);
    }
    fclose(rem_file);
    return NULL;
}

void* pref_task(void *tnum){
    int t = *((int *) tnum);
    char key[15];
    char pref_test_name[100];
    sprintf(pref_test_name,"./workload/pref/%d.txt",t);
    FILE* pref_test = fopen(pref_test_name,"r");
    while (1)
    {
        char** list;
        fscanf(pref_test,"%s",key);
        if (key[0]=='-') break;
        list = keys_with_prefix(trie, key);
        free(list);
    }
    fclose(pref_test);
    return NULL;
}

void ins_parent(){
    pthread_t ins_thread[NUMBER_OF_CONCURRENT_THREADS];

    for(int i=0;i<NUMBER_OF_CONCURRENT_THREADS; i++){
        pthread_create(&ins_thread[i],NULL, insert_task, (void *)& narr[i]);
    }
    for(int i=0;i<NUMBER_OF_CONCURRENT_THREADS; i++){
        pthread_join(ins_thread[i],NULL);
    }
}

void find_parent(int nthread){
    pthread_t find_thread[nthread];

    for(int i=0;i<nthread; i++){
        pthread_create(&find_thread[i],NULL, insert_task, (void *)& narr[i]);
    }
    for(int i=0;i<nthread; i++){
        pthread_join(find_thread[i],NULL);
    }
}

void rem_parent(int nthread){
    pthread_t rem_thread[nthread];

    for(int i=0;i<nthread; i++){
        pthread_create(&rem_thread[i],NULL, insert_task, (void *)& narr[i]);
    }
    for(int i=0;i<nthread; i++){
        pthread_join(rem_thread[i],NULL);
    }
}

void pref_parent(int nthread){
    pthread_t pref_thread[nthread];

    for(int i=0;i<nthread; i++){
        pthread_create(&pref_thread[i],NULL, insert_task, (void *)& narr[i]);
    }
    for(int i=0;i<nthread; i++){
        pthread_join(pref_thread[i],NULL);
    }
}

int main(int argc, char* argv[]){
    //Test begins
    
    char key[MAX_TREE_SIZE +5];
    for(int i=0; i<NUMBER_OF_CONCURRENT_THREADS; i++){
        narr[i] = i+1;
    }

    struct timeval start, end; 

    char path[1024] = "./data/nthreads_s_lock.csv";

    FILE * f = fopen(path, "w");

    if(f == NULL){
        printf("Could not create the CSV file %s\n", path);
        return 0;
    }

    for(int i=0; i<NUMBER_OF_CONCURRENT_THREADS; i++){
        
        // start timer. 
        gettimeofday(&start, NULL);

        trie = init_trie();

        ins_parent(i);
        find_parent(i);
        rem_parent(i);
        pref_parent(i);

        gettimeofday(&end, NULL);

        // long long elapsed = (end.tv_sec-start.tv_sec)*1000000LL + end.tv_usec-end.tv_usec;
        double time_taken; 
  
        time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
        time_taken = (time_taken + (end.tv_usec -start.tv_usec)) * 1e-6; 

        fprintf(f,"%d,%f\n",i+1, time_taken);

        delete_trie(trie);
    }

    fclose(f);

}