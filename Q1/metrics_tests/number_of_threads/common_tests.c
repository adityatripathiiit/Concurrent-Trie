#include "../../trie.c"
#include "../../trie.h"
#define NUMBER_OF_CONCURRENT_THREADS 100

trie_t trie; 

FILE* ins_file ;
FILE* find_test;
FILE* rem_file; 
FILE* pref_test; 

pthread_mutex_t ins_file_lock; 

void* insert_task(){
    char key[15];
    int val;
    while (1)
    {
        pthread_mutex_lock(&ins_file_lock);
        if(fscanf(ins_file,"%s",key) == EOF){
            pthread_mutex_unlock(&ins_file_lock);
            break;
        }
        if (key[0]=='-' || !ins_file){
            pthread_mutex_unlock(&ins_file_lock);
            break;
        }
        fscanf(ins_file,"%d",&val);
        pthread_mutex_unlock(&ins_file_lock);
        insert(trie,key,val);
    }
    return NULL;
}

void* find_task(){
   
    char key[15];
    while (1)
    {
        int f_ret = -101;
        if(fscanf(find_test,"%s",key) == EOF) break;
        if (key[0]=='-' || !find_test) break;
        find(trie,key,&f_ret);
    }
    return NULL;
}

void *rem_task(){
    char key[15];

    while (rem_file)
    {
        if (fscanf(rem_file,"%s",key) == EOF) break;
        if (key[0]=='-' || !rem_file) break;
        delete_kv(trie,key);
    }
    return NULL;
}

void* pref_task(){
    char key[15];
    while (pref_test)
    {
        char** list;
        if (fscanf(pref_test,"%s",key) == EOF) break;
        if (key[0]=='-' || !pref_test) break;
        list = keys_with_prefix(trie, key);
        free(list);
    }
    return NULL;
}

void ins_parent(int nthread){
    pthread_mutex_init(&ins_file_lock, NULL) ;
    ins_file = fopen("./workload/initial/1.txt","r");

    pthread_t ins_thread[nthread];

    for(int i=0;i<nthread; i++){
        pthread_create(&ins_thread[i],NULL, insert_task,NULL);
    }
    for(int i=0;i<nthread; i++){
        pthread_join(ins_thread[i],NULL);
    }

    fclose(ins_file);
}

void find_parent(int nthread){
    find_test = fopen("./workload/find/1.txt", "r");

    pthread_t find_thread[nthread];

    for(int i=0;i<nthread; i++){
        pthread_create(&find_thread[i],NULL, find_task, NULL);
    }
    for(int i=0;i<nthread; i++){
        pthread_join(find_thread[i],NULL);
    }
    fclose(find_test);
}

void rem_parent(int nthread){
    rem_file = fopen("./workload/rem/1.txt", "r");

    pthread_t rem_thread[nthread];

    for(int i=0;i<nthread; i++){
        pthread_create(&rem_thread[i],NULL, rem_task,NULL);
    }
    for(int i=0;i<nthread; i++){
        pthread_join(rem_thread[i],NULL);
    }
    fclose(rem_file);
}

void pref_parent(int nthread){
    pref_test = fopen("./workload/pref/1.txt", "r");
    
    pthread_t pref_thread[nthread];

    for(int i=0;i<nthread; i++){
        pthread_create(&pref_thread[i],NULL, pref_task,NULL);
    }
    for(int i=0;i<nthread; i++){
        pthread_join(pref_thread[i],NULL);
    }
    fclose(pref_test);
}
