#include "../../trie.c"
#include "../../trie.h"
#define SIZE_OF_TREE 30 
#define NUMBER_OF_FIXED_THREADS 30

trie_t trie; 
// int narr[NUMBER_OF_CONCURRENT_THREADS];
pthread_mutex_t ins_file_lock; 

FILE* ins_file ;
FILE* find_test;
FILE* rem_file; 
FILE* pref_test; 

void* insert_task(){
    // int t = *((int *) tnum);
    // FILE* ins_file = fopen("./workload/initial/1.txt","r");

    char key[SIZE_OF_TREE+10];
    int val;
    // char ins_file_name[100];
    // sprintf(ins_file_name, "./workload/initial/%d.txt",t);
    // FILE* ins_file = fopen(ins_file_name,"r");
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
    // fclose(ins_file);
    return NULL;
}

void* find_task(){
    // int t = *((int *) tnum);
    char key[SIZE_OF_TREE+10];
    // char find_test_name[100];
    // sprintf(find_test_name,"./workload/find/%d.txt",t);
    // FILE* find_test = fopen(find_test_name,"r");
    while (1)
    {
        int f_ret = -101;
        if(fscanf(find_test,"%s",key) == EOF) break;
        if (key[0]=='-' || !find_test) break;
        find(trie,key,&f_ret);
    }
    // fclose(find_test);
    return NULL;
}

void *rem_task(){

    char key[SIZE_OF_TREE+100];
    // int t = *((int *) tnum);
    // char rem_file_name[100];
    // sprintf(rem_file_name,"./workload/rem/%d.txt",t);
    // FILE* rem_file = fopen(rem_file_name,"r");
    
    while (1)
    {
        if (fscanf(rem_file,"%s",key) == EOF) break;
        if (key[0]=='-' || !rem_file) break;
        delete_kv(trie,key);
    }
    // fclose(rem_file);
    return NULL;
}

void* pref_task(){
    char key[SIZE_OF_TREE+10];
    // int t = *((int *) tnum);
    // char pref_test_name[100];
    // sprintf(pref_test_name,"./workload/pref/%d.txt",t);
    // FILE* pref_test = fopen(pref_test_name,"r");
    while (pref_test)
    {
        char** list;
        if (fscanf(pref_test,"%s",key) == EOF) break;
        if (key[0]=='-' || !pref_test) break;
        list = keys_with_prefix(trie, key);
        free(list);
    }
    // fclose(pref_test);
    return NULL;
}

void ins_parent(int nthread){
    
    pthread_mutex_init(&ins_file_lock, NULL) ;
    char ins_file_name[1000];
    sprintf(ins_file_name, "./workload/initial/%d.txt",nthread);
    

    ins_file = fopen(ins_file_name,"r");

    pthread_t ins_thread[NUMBER_OF_FIXED_THREADS];

    for(int i=0;i<NUMBER_OF_FIXED_THREADS; i++){
        pthread_create(&ins_thread[i],NULL, insert_task,NULL);
    }
    for(int i=0;i<NUMBER_OF_FIXED_THREADS; i++){
        pthread_join(ins_thread[i],NULL);
    }

    fclose(ins_file);
}

void find_parent(int nthread){

    char find_file_name[100];
    sprintf(find_file_name, "./workload/find/%d.txt",nthread);


    find_test = fopen(find_file_name,"r");


    pthread_t find_thread[NUMBER_OF_FIXED_THREADS];

    for(int i=0;i<NUMBER_OF_FIXED_THREADS; i++){
        pthread_create(&find_thread[i],NULL, find_task, NULL);
    }
    for(int i=0;i<NUMBER_OF_FIXED_THREADS; i++){
        pthread_join(find_thread[i],NULL);
    }
    fclose(find_test);
}

void rem_parent(int nthread){

    
    char rem_file_name[100];
    sprintf(rem_file_name, "./workload/rem/%d.txt",nthread);
    rem_file = fopen(rem_file_name,"r");

    pthread_t rem_thread[NUMBER_OF_FIXED_THREADS];

    for(int i=0;i<NUMBER_OF_FIXED_THREADS; i++){
        pthread_create(&rem_thread[i],NULL, rem_task,NULL);
    }
    for(int i=0;i<NUMBER_OF_FIXED_THREADS; i++){
        pthread_join(rem_thread[i],NULL);
    }
    fclose(rem_file);
}

void pref_parent(int nthread){

    char pref_file_name[100];
    sprintf(pref_file_name, "./workload/pref/%d.txt",nthread);
    pref_test = fopen(pref_file_name,"r");
    
    pthread_t pref_thread[NUMBER_OF_FIXED_THREADS];

    for(int i=0;i<NUMBER_OF_FIXED_THREADS; i++){
        pthread_create(&pref_thread[i],NULL, pref_task,NULL);
    }
    for(int i=0;i<NUMBER_OF_FIXED_THREADS; i++){
        pthread_join(pref_thread[i],NULL);
    }
    fclose(pref_test);
}
