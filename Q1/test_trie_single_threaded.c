#define _S_LOCK_TRIE
#define _NO_HOH_LOCK_TRIE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "trie.c"

#define RESET   "\033[0m"
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define RED     "\033[31m"      /* Red */

void test_file_diff(char* f1, char* f2){
    char command[200];
    sprintf(command,"cmp -s %s %s && echo \"\\033[32mPassed\" || echo \"\\033[31mFailure\"",f1, f2);
    system(command);
}

int main(int argc, char* argv[]){
    // Single threaded trie test begins
    trie_t trie = init_trie();

    // Inserting
    printf(YELLOW "Testing Insert..." RESET);
    char key[15];
    int val;
    char * ins_file_name = "./tests/single_thread/initial.txt";
    FILE* ins_file = fopen(ins_file_name,"r");
    while (1)
    {
        fscanf(ins_file,"%s",key);
        if (key[0]=='-') break;
        fscanf(ins_file,"%d",&val);
        insert(trie,key,val);
    }
    fclose(ins_file);
    
    // Test trie state after insert
    char * ins_exp_name = "./tests/single_thread/exp_ins.txt";
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
    printf(YELLOW "Testing Find..." RESET);
    fflush(stdout);
    char * find_test_name = "./tests/single_thread/find_test.txt";
    FILE* find_test = fopen(find_test_name,"r");
    char* find_test_exp_name = "./.testout/find_test_exp.txt";
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
    test_file_diff("./tests/single_thread/find_test_exp.txt",find_test_exp_name);

    // Removing
    printf(YELLOW "Testing Delete..." RESET);
    char * rem_file_name = "./tests/single_thread/rem_list.txt";
    FILE* rem_file = fopen(rem_file_name,"r");
    while (1)
    {
        fscanf(rem_file,"%s",key);
        if (key[0]=='-') break;
        delete_kv(trie,key);
    }
    fclose(rem_file);
    
    // Test trie state after remove
    char * rem_exp_name = "./tests/single_thread/exp_rem.txt";
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
    if(err==1) printf(RED "Failure\n" RESET);
    else printf(GREEN "Passed\n" RESET);

    // Prefix searching
    printf(YELLOW "Testing Prefix..." RESET);
    fflush(stdout);
    char * pref_test_name = "./tests/single_thread/pref_text.txt";
    FILE* pref_test = fopen(pref_test_name,"r");
    char* pref_test_exp_name = "./.testout/pref_text_exp.txt";
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
    test_file_diff("./tests/single_thread/pref_text_exp.txt",pref_test_exp_name);

    // Delete the trie
    delete_trie(trie);
    printf(GREEN "Single threaded trie implementation test completed.\n" RESET);
    return 0;
}