#define _S_LOCK_TRIE
#define _NO_HOH_LOCK_TRIE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "trie.c"

#define RESET   "\033[0m"
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define RED     "\033[31m"      /* Red */
#define ff fflush(stdout)

trie_t trie;

void test_file_diff(char* f1, char* f2){
    char command[200];
    sprintf(command,"cmp -s %s %s && echo \"\\033[32mPassed\" || echo \"\\033[31mFailure\"",f1, f2);
    system(command);
}

void* insert_task(void *tnum){
    int t = *((int *) tnum);
    char key[15];
    int val;
    char ins_file_name[100];
    sprintf(ins_file_name, "./tests/multi_thread/initial/%d.txt",t);
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
    sprintf(find_test_name,"./tests/multi_thread/find/%d.txt",t);
    FILE* find_test = fopen(find_test_name,"r");
    char find_test_exp_name[100];
    sprintf(find_test_exp_name,"./.testout/exp_find_%d.txt",t);
    FILE* find_test_exp = fopen(find_test_exp_name,"w");
    while (1)
    {
        int f_ret = -101;
        fscanf(find_test,"%s",key);
        if (key[0]=='-') break;
        if(find(trie,key,&f_ret)==-1) fprintf(find_test_exp,"NO\n");
        else fprintf(find_test_exp,"YES\n");
    }
    fclose(find_test);
    fclose(find_test_exp);
    return NULL;
}

void *rem_task(void *tnum){
    int t = *((int *) tnum);
    char key[15];
    char rem_file_name[100];
    sprintf(rem_file_name,"./tests/multi_thread/rem/%d.txt",t);
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
    sprintf(pref_test_name,"./tests/multi_thread/pref/%d.txt",t);
    FILE* pref_test = fopen(pref_test_name,"r");
    char pref_test_exp_name[100];
    sprintf(pref_test_exp_name,"./.testout/pref_text_exp_%d.txt",t);
    FILE* pref_test_exp = fopen(pref_test_exp_name,"w");
    while (1)
    {
        char** list;
        fscanf(pref_test,"%s",key);
        if (key[0]=='-') break;
        list = keys_with_prefix(trie, key);
        int i=0;
        if (list[i]==NULL){
            fprintf(pref_test_exp,"\n");
        }
        while(list[i]!=NULL){
            fprintf(pref_test_exp,"%s\n",list[i]);
            free(list[i]);
            i++;
        }
        free(list);
    }
    fclose(pref_test);
    fclose(pref_test_exp);
    return NULL;
}

int main(int argc, char* argv[]){
    // Trie test begins
    trie = init_trie();
    char key[15];
    int val;
    int narr[5] = {0,1,2,3,4};

    // Inserting
    printf(YELLOW "Testing Insert..." RESET);
    pthread_t ins_threads[3];
    pthread_create(&ins_threads[0], NULL, insert_task,(void *)&narr[1]);
    pthread_create(&ins_threads[1], NULL, insert_task,(void *)&narr[2]);
    pthread_create(&ins_threads[2], NULL, insert_task,(void *)&narr[3]);

    pthread_join(ins_threads[0],NULL);
    pthread_join(ins_threads[1],NULL);
    pthread_join(ins_threads[2],NULL);

    // Test trie state after insert
    char * ins_exp_name = "./tests/multi_thread/initial/exp_ins.txt";
    FILE* ins_exp = fopen(ins_exp_name,"r");
    int err = 0;
    while (1)
    {
        int f_ret = -101;
        fscanf(ins_exp,"%s",key);
        if (key[0]=='-') break;
        fscanf(ins_exp,"%d",&val);
        find(trie,key,&f_ret);
        if (f_ret!=val){
            err = 1;
            break;
        }
    }
    fclose(ins_exp);
    if(err==1) printf(RED "Failure\n" RESET);
    else printf(GREEN "Passed\n" RESET);

    // Test find
    printf(YELLOW "Testing Find...\n" RESET);ff;
    pthread_t find_threads[3];
    pthread_create(&find_threads[0], NULL, find_task,(void *)&narr[1]);
    pthread_create(&find_threads[1], NULL, find_task,(void *)&narr[2]);
    pthread_create(&find_threads[2], NULL, find_task,(void *)&narr[3]);

    pthread_join(find_threads[0],NULL);
    pthread_join(find_threads[1],NULL);
    pthread_join(find_threads[2],NULL);
    printf(YELLOW "Thread 1: " RESET);ff;
    test_file_diff("./tests/multi_thread/find/exp_find_1.txt","./.testout/exp_find_1.txt");
    printf(YELLOW "Thread 2: " RESET);ff;
    test_file_diff("./tests/multi_thread/find/exp_find_2.txt","./.testout/exp_find_2.txt");
    printf(YELLOW "Thread 3: " RESET);ff;
    test_file_diff("./tests/multi_thread/find/exp_find_3.txt","./.testout/exp_find_3.txt");

    // Removing
    printf(YELLOW "Testing Delete..." RESET);ff;
    pthread_t rem_threads[3];
    pthread_create(&rem_threads[0], NULL, rem_task,(void *)&narr[1]);
    pthread_create(&rem_threads[1], NULL, rem_task,(void *)&narr[2]);
    pthread_create(&rem_threads[2], NULL, rem_task,(void *)&narr[3]);

    pthread_join(rem_threads[0],NULL);
    pthread_join(rem_threads[1],NULL);
    pthread_join(rem_threads[2],NULL);
    
    // Test trie state after remove
    char * rem_exp_name = "./tests/multi_thread/rem/exp.txt";
    FILE* rem_exp = fopen(rem_exp_name,"r");
    err = 0;
    while (1)
    {
        int f_ret = -101;
        fscanf(rem_exp,"%s",key);
        if (key[0]=='-') break;
        fscanf(rem_exp,"%d",&val);
        find(trie,key,&f_ret);
        if (f_ret!=val){
            err = 1;
            break;
        }
    }
    fclose(rem_exp);
    if(err==1) {printf(RED "Failure\n" RESET);ff;}
    else {printf(GREEN "Passed\n" RESET);ff;}

    // Prefix searching
    printf(YELLOW "Testing Prefix...\n" RESET);ff;
    fflush(stdout);
    pthread_t pref_threads[3];
    pthread_create(&pref_threads[0], NULL, pref_task,(void *)&narr[1]);
    pthread_create(&pref_threads[1], NULL, pref_task,(void *)&narr[2]);
    pthread_create(&pref_threads[2], NULL, pref_task,(void *)&narr[3]);

    pthread_join(pref_threads[0],NULL);
    pthread_join(pref_threads[1],NULL);
    pthread_join(pref_threads[2],NULL);
    printf(YELLOW "Thread 1: " RESET);ff;
    test_file_diff("./tests/multi_thread/pref/exp_1.txt","./.testout/pref_text_exp_1.txt");
    printf(YELLOW "Thread 2: " RESET);ff;
    test_file_diff("./tests/multi_thread/pref/exp_2.txt","./.testout/pref_text_exp_2.txt");
    printf(YELLOW "Thread 3: " RESET);ff;
    test_file_diff("./tests/multi_thread/pref/exp_3.txt","./.testout/pref_text_exp_3.txt");

    // Remove + Find Test
    printf(YELLOW "Testing Find + Delete..." RESET);ff;
    insert_task(&narr[1]);
    insert_task(&narr[2]);
    insert_task(&narr[3]);
    pthread_t frem_threads[4];
    pthread_create(&frem_threads[0], NULL, rem_task,(void *)&narr[1]);
    pthread_create(&frem_threads[1], NULL, rem_task,(void *)&narr[2]);
    pthread_create(&frem_threads[2], NULL, rem_task,(void *)&narr[3]);
    pthread_create(&frem_threads[3], NULL, find_task,(void *)&narr[1]);

    pthread_join(frem_threads[0],NULL);
    pthread_join(frem_threads[1],NULL);
    pthread_join(frem_threads[2],NULL);
    pthread_join(frem_threads[3],NULL);

    // Test trie state after remove
    rem_exp = fopen(rem_exp_name,"r");
    err = 0;
    while (1)
    {
        int f_ret = -101;
        fscanf(rem_exp,"%s",key);
        if (key[0]=='-') break;
        fscanf(rem_exp,"%d",&val);
        find(trie,key,&f_ret);
        if (f_ret!=val){
            err = 1;
            break;
        }
    }
    fclose(rem_exp);
    if(err==1) {printf(RED "Failure\n" RESET);ff;}
    else {printf(GREEN "Passed\n" RESET);ff;}

    // Delete the trie
    delete_trie(trie);
    printf(GREEN "Multi threaded trie implementation test completed.\n" RESET);
    return 0;
}