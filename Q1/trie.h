#ifndef TRIE_H
#define TRIE_H
#include <stdbool.h>
#include <pthread.h>

// Alphabet size (# of symbols) 
#define ALPHABET_SIZE (26) 

// Converts key current character into index 
// use only 'a' through 'z' and lower case 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 


typedef struct node { 
    int value;
    bool is_end;
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_t node_lock;
    #endif
    struct node* children[26];   // Array mapping character to the next node
} _trie_node_t;

typedef _trie_node_t* trie_node_t;

typedef struct {
    trie_node_t head; // The head/start node of trie
    // Add whatever variables you require for locking here
} _trie_t;

typedef _trie_t* trie_t;

/* 
    Your task is to define the implementations of the following function in trie.c.
    You can write as many auxilary/helper functions as you want in trie.c. 
    Compiling the test code:
        `make test_trie_single_threaded`
        `make test_trie_s_lock` 
        `make test_trie_rw_lock`

    Compiling and running the tests:
        `make single_threaded`
        `make s_lock` 
        `make rw_lock`
*/

// init_trie() should initialize and return an empty trie
trie_t init_trie(void); 

/* 
    insert(trie_t trie, char* key, int value) should put the given value in the trie t.
    Overwrite the value if the key already exists.
*/
void insert(trie_t trie, char* key, int value); 

/* 
    find(trie_t trie, char* key, void **save_ptr) should return -1 if the key is not found. 
    Otherwise, return 0 and put the value in the variable pointed to by save_ptr.
*/
int find(trie_t trie, char* key, int* val_ptr); 

/*
    delete_kv(trie_t trie, char* key) deletes the key and its value from the trie if it is present.
*/
void delete_kv(trie_t trie, char* key);

/*
    keys_with_prefix(trie_t trie, char* prefix) should return an array of strings with the given prefix (in sorted order).
    Last element of the array should be NULL
    If no key matches the prefix, the array will just have a single NULL.
*/
char** keys_with_prefix(trie_t trie, char* prefix);

/*
    delete_trie(trie_t trie) clears the entire trie from memory. 
*/
void delete_trie(trie_t trie);


#endif