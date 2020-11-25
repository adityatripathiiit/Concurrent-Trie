#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include <string.h>
#include <stdbool.h> 
#include "common_threads.h"


pthread_mutex_t global_lock;
pthread_rwlock_t wr_lock;

/*  init_trie() should initialize a trie,
	and respective locks for all the HOH,Single Lock case and RW lock case.
 	Return return an empty trie
*/
trie_t init_trie(void){

	// Initializing the trie
    _trie_t* trie =  ( _trie_t*) malloc(sizeof( _trie_t)); 

	// If malloc didn't worked correctly, return
	if(trie == NULL){
		printf("unable to create trie\n"); 
		return NULL; 
	}
	// Initializing the Head node of the trie
    trie_node_t head = (trie_node_t)malloc(sizeof(_trie_node_t)); 

	if(head == NULL){
		printf("unable to create trie head\n"); 
		return NULL; 
	}

    trie->head = head ;
	trie->head->is_end = false;

	//  Initializing all the children nodes of trie head node to NULL
	for (int i = 0; i < ALPHABET_SIZE; i++) 
		trie->head->children[i] = NULL; 

	// Initializing the lock of the head node of the trie, If HOH lock case
	#ifndef _NO_HOH_LOCK_TRIE
	Pthread_mutex_init(&trie->head->node_lock,NULL); 
	#endif


	// Initializing the global lock in case of single lock
	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
	Pthread_mutex_init(&global_lock, NULL) ;
	#endif
	#endif

	// Initializing the global lock in case of reader writer lock

	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_init(&wr_lock, NULL); 
	#endif 
	#endif

	return trie; 
} 

/* 
    create_node(void) should create a new trie_node and initialise its locks if HOH case 
    It will also initialise all the children of the new node to NULL
	Finally return the trie_node.
*/

struct node* create_node(void) {
    trie_node_t  trie_node = (trie_node_t)malloc(sizeof(_trie_node_t)); 
	if(trie_node == NULL) {
		printf("Unable to create trie node\n"); 
		return NULL; 
	}

    trie_node->is_end = false ;
    for (int i = 0; i < ALPHABET_SIZE; i++) 
		trie_node->children[i] = NULL; 

	#ifndef _NO_HOH_LOCK_TRIE
	Pthread_mutex_init(&trie_node->node_lock,NULL); 
	#endif
	
    return trie_node; 

}

/* 
    isLastNode(struct node* trie_node) should return 0 if current node has a child
	Otherwise  return 1 signifying that the given trie node is the last node
*/

bool isLastNode(struct node* trie_node){ 

	for (int i = 0; i < ALPHABET_SIZE; i++){
		if (trie_node->children[i]){ 
			return 0; 
		}
	}
	return 1; 
} 

/* 
    insert(trie_t trie, char* key, int value) should put the given value in the trie t.
    Overwrite the value if the key already exists.
*/
void insert(trie_t trie, char* key, int value){
    
	// Acquiring the global lock if S_LOCK case
	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
	Pthread_mutex_lock(&global_lock)  ;
	#endif 
	#endif

	// Acquiring the ReaderWriter if RW_LOCK case
	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_wrlock(&wr_lock); 
	#endif 
	#endif 

	// Head node of the trie
	struct node* ins_node = trie->head;

	// Acquiring the lock of head node if HOH_LOCK case
	#ifndef _NO_HOH_LOCK_TRIE
	Pthread_mutex_lock(&ins_node->node_lock); 
	
	#endif

	// Iterating through the key and checking if the node curresponding current key[level] is in the trie
	// if not then create a new node corresponding to key[level]
	for (int level = 0; level < strlen(key); level++) 
	{ 
		int index = CHAR_TO_INDEX(key[level]); 
		if (ins_node->children[index] == NULL){ 
			ins_node->children[index] = create_node();
		}

		// Using HOH technique. Creating the temporary node equal to ins_node
		// Currently the lock of ins_node is acquried
		#ifndef _NO_HOH_LOCK_TRIE
		struct node* temp = ins_node;
		#endif
		
		// moving the ins_node to its children
		ins_node = ins_node->children[index]; 

		// Acquire the lock in handover hand fashion 
		// locking the next node's lock and releasing the previous node's lock
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_lock(&ins_node->node_lock); 
		Pthread_mutex_unlock(&temp->node_lock);
		#endif 
	
	} 
	// Replacing the value of last node entry with the new value
    ins_node->value = value ; 

	// mark last node as leaf 
	ins_node->is_end = true; 

	// Unlocking the current acquired lock in HOH case 
	#ifndef _NO_HOH_LOCK_TRIE
	Pthread_mutex_unlock(&ins_node->node_lock); 
	#endif 
	// Unlocking the global lock in case of S_LOCK
	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
	Pthread_mutex_unlock(&global_lock);
	#endif 
	#endif 
	// Unlocking the rwlock lock in case of RW_LOCK 
	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_unlock(&wr_lock); 
	#endif 
	#endif 

}

/* 
    find(trie_t trie, char* key, void **save_ptr) should return -1 if the key is not found. 
    Otherwise, return 0 and put the value in the variable pointed to by save_ptr.
*/
int find(trie_t trie,char* key, int* val_ptr){
    // Write your code here
    
	int length = strlen(key); 
    
	// If head is NULL return not found
	if(trie->head == NULL) return -1; 
	
	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
	Pthread_mutex_lock(&global_lock);
	#endif 
	#endif

	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_rdlock(&wr_lock); 
	#endif 
	#endif 

	struct node* itr_node = trie->head ; 

	#ifndef _NO_HOH_LOCK_TRIE
	Pthread_mutex_lock(&itr_node->node_lock); 
	#endif

	for (int level = 0; level < length; level++) 
	{ 

		int index = CHAR_TO_INDEX(key[level]); 
		// If at any point there is no node corresponding to key[level] in the current trie branch, 
		// then release all the acquired locks and return not found
		if (!itr_node->children[index]){
			#ifdef _NO_HOH_LOCK_TRIE
			#ifdef _S_LOCK_TRIE 
			Pthread_mutex_unlock(&global_lock);
			#endif 
			#endif 

			#ifdef _NO_HOH_LOCK_TRIE
			#ifndef _S_LOCK_TRIE 
			pthread_rwlock_unlock(&wr_lock); 
			#endif 
			#endif 

			#ifndef _NO_HOH_LOCK_TRIE
			Pthread_mutex_unlock(&itr_node->node_lock); 
			#endif

			return -1; 
		} 
		// If child node is present, then keep on moving in the trie branch till we reach at the correct node
		// so that the path we took matches key value when concatenated.
		// Acquire and releasing the locks in HOH fashion
		#ifndef _NO_HOH_LOCK_TRIE
		struct node* temp = itr_node;
		#endif
		itr_node = itr_node->children[index]; 
		
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_lock(&itr_node->node_lock); 
		Pthread_mutex_unlock(&temp->node_lock);
		#endif 

	} 

	// If the current node is the end of a word, then release all the locks 
	// And return found
    if(itr_node != NULL && itr_node->is_end){
        *val_ptr = itr_node->value ; 
		
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_unlock(&itr_node->node_lock);
		#endif

		#ifdef _NO_HOH_LOCK_TRIE
		#ifdef _S_LOCK_TRIE 	
		Pthread_mutex_unlock(&global_lock);
		#endif 
		#endif 

		#ifdef _NO_HOH_LOCK_TRIE
		#ifndef _S_LOCK_TRIE 
		pthread_rwlock_unlock(&wr_lock); 
		#endif 
		#endif

        return 0 ; 
    }

	// If either the current node is null or not an end of word, then release all the locks
	// And return not found
	#ifndef _NO_HOH_LOCK_TRIE
	Pthread_mutex_unlock(&itr_node->node_lock);
	#endif

	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_unlock(&wr_lock); 
	#endif 	
	#endif

	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
	Pthread_mutex_unlock(&global_lock);
	#endif 
	#endif

    return -1;
} 

/*
    delete_kv_helper(struct node* itr_node, char* key) deletes the key and its value from the trie if it is present.
	Helper function for delete_kv
	Since it is a recursive function, therefore in HOH case, when a thread will be coming up, and some thread will be going down a 
	branch, then there might be deadlock conditions. Therefore here we lock the complete path of the trie, as we move forward, so that no 
	deadlock conditions are there. 
*/

struct node* delete_kv_helper(struct node* itr_node, char *key){
	
	if(itr_node == NULL){
		return itr_node;
	}
	// Locking the path
	#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_lock(&itr_node->node_lock);
	#endif

	// If key has not ended
	if(*key){
		// Setting the value of child node recursively
		itr_node->children[(CHAR_TO_INDEX(*key))] = delete_kv_helper(itr_node->children[CHAR_TO_INDEX(*key)], key+1);

		if(itr_node->is_end == 0 && isLastNode(itr_node)){
				// If the node is the leaf node, then release the lock and destory the lock in case of HOH
				// Then free the node and return NULL to its parent
				#ifndef _NO_HOH_LOCK_TRIE
				Pthread_mutex_unlock(&itr_node->node_lock);
				pthread_mutex_destroy(&itr_node->node_lock);
				#endif
				free(itr_node);
				itr_node = NULL;
				
				return itr_node;
		}
		// If the current node is not the last node, then simply unlock the lock and return this node 
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_unlock(&itr_node->node_lock);
		#endif

		return itr_node;
	}
		
	// if the key has ended 
	else if(*key == '\0'){
		
		itr_node->is_end = 0;

		if(isLastNode(itr_node)){
			#ifndef _NO_HOH_LOCK_TRIE
			Pthread_mutex_unlock(&itr_node->node_lock);
			pthread_mutex_destroy(&itr_node->node_lock);
			#endif
			free(itr_node);
			itr_node = NULL;
			return itr_node;
		}
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_unlock(&itr_node->node_lock);
		#endif

		return itr_node;
	}


	return itr_node;
	
}

/*
    delete_kv(trie_t trie, char* key) deletes the key and its value from the trie if it is present.
*/

void delete_kv(trie_t trie, char* key){
		// Acquire the global lock in case of single lock trie
		#ifdef _NO_HOH_LOCK_TRIE
		#ifdef _S_LOCK_TRIE 
		Pthread_mutex_lock(&global_lock);
		#endif 
		#endif
		// Acquire the reader writer lock in case of rwlock trie
		#ifdef _NO_HOH_LOCK_TRIE
		#ifndef _S_LOCK_TRIE 
		pthread_rwlock_wrlock(&wr_lock); 
		#endif 
		#endif

    	struct node* itr_node = trie->head; 
		// Acquire the lock of trie head in case of HOH trie
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_lock(&itr_node->node_lock ); 
		#endif

		// Send the child of the head node who matches the corresponding key's first character
		// A way to ensure that trie's head is never set to NULL
		for( int i=0; i <ALPHABET_SIZE; i++){
			if((*key == i+'a') && itr_node->children[i]){
				itr_node->children[i] =  delete_kv_helper (itr_node->children[i], key+1);
			}
		}

		// Releasing respective acquired locks
		#ifdef _NO_HOH_LOCK_TRIE
		#ifdef _S_LOCK_TRIE 
		Pthread_mutex_unlock(&global_lock);
		#endif 
		#endif 

		#ifdef _NO_HOH_LOCK_TRIE
		#ifndef _S_LOCK_TRIE 
		pthread_rwlock_unlock(&wr_lock); 
		#endif 
		#endif 
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_unlock(&itr_node->node_lock ); 
		#endif

		return ; 
}




/*
    suggestionsRec(struct node* trie_node, char* currPrefix, char** list, int * INS_INDEX) 
    Recursive function to find auto-suggestions for given node. 
	Since it is a recursive function, therefore in HOH case, when a thread will be coming up the trie and of the recursion, and some thread will be going down a 
	branch, then there might be deadlock conditions. Therefore here we lock the complete path of the trie, as we move forward, so that no 
	deadlock conditions are there
*/


void suggestionsRec(struct node* trie_node, char* currPrefix, char** list, int * INS_INDEX) 
{ 
	// Found a string in Trie with the given prefix 
	// Insert in the list and increament the count of INS_INDEX
	if (trie_node->is_end) 
	{ 
		list[*INS_INDEX] = malloc((strlen(currPrefix)+2)*sizeof(char)); 
		strcpy(list[*INS_INDEX], currPrefix); 
		(*INS_INDEX)++; 
	} 

	// All children struct node pointers are NULL then return
	if (isLastNode(trie_node)) 
		return; 

	// Recursively finding prefix, in a sorted oreder.
	// Locking the complete path in a branch and releasing it while returning above
	for (int i = 0; i < ALPHABET_SIZE; i++) 
	{ 
		if (trie_node->children[i]!= NULL) 
		{ 
			struct node* child = trie_node->children[i];
			#ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_lock(&child->node_lock);
			#endif


			char *str = malloc (sizeof (char) * (strlen (currPrefix) + 2));
			strcpy(str,currPrefix);
			int j = strlen(currPrefix);
			str[j++] = i+97;
			str[j] = '\0';

			// #ifndef _NO_HOH_LOCK_TRIE
            // pthread_mutex_unlock(&trie_node->node_lock);
			// #endif
			suggestionsRec(child, str, list,INS_INDEX); 

			#ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_unlock(&child->node_lock);
			// pthread_mutex_lock(&trie_node->node_lock);
			#endif

			free(str);
		} 

	}
}

/*
    keys_with_prefix(trie_t trie, char* prefix) should return an array of strings with the given prefix (in sorted order).
    Last element of the array should be NULL
    If no key matches the prefix, the array will just have a single NULL.
*/

char** keys_with_prefix(trie_t trie, char* prefix){
    // Write your code here

	// Insert index pointer to maintain the index of insertion in the character array
    int* INS_INDEX = malloc(sizeof(int)); 
	*INS_INDEX = 0 ;
	// Initilizing the character array
    char** list = malloc(2048*sizeof(char*));  
	list[*INS_INDEX] = NULL;
	if(list== NULL){
		printf("cant malloc "); 
		free(INS_INDEX);
		return list; 
	}
	// Acquiring lock as described in find. 
	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
	Pthread_mutex_lock(&global_lock);
	#endif 
	#endif

	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_rdlock(&wr_lock); 
	#endif 
	#endif 
	
	struct node* itr_node = trie->head; 

	#ifndef _NO_HOH_LOCK_TRIE
	Pthread_mutex_lock(&itr_node->node_lock); 
	#endif 
		
	if(itr_node == NULL){

		#ifdef _S_LOCK_TRIE
		Pthread_mutex_unlock(&global_lock);
		#endif 
	
		#ifdef _NO_HOH_LOCK_TRIE
		#ifndef _S_LOCK_TRIE 
		pthread_rwlock_unlock(&wr_lock); 
		#endif 
		#endif 

		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_unlock(&itr_node->node_lock); 
		#endif 

		free(INS_INDEX)	;
		return list; 
	}

	// Check if prefix is present and find the 
	// the node (of last level) with last character 
	// of given string. 
	// Steps similar to find operation
	int level; 
	int n = strlen(prefix); 
	for (level = 0; level < n; level++) 
	{ 
		int index = CHAR_TO_INDEX(prefix[level]); 
		
		// no string in the Trie has this prefix 
		if (!itr_node->children[index]) {
			#ifdef _NO_HOH_LOCK_TRIE
			#ifdef _S_LOCK_TRIE 
			Pthread_mutex_unlock(&global_lock);
			#endif 
			#endif 

			#ifdef _NO_HOH_LOCK_TRIE
			#ifndef _S_LOCK_TRIE 
			pthread_rwlock_unlock(&wr_lock); 
			#endif 
			#endif 
			
			#ifndef _NO_HOH_LOCK_TRIE
			Pthread_mutex_unlock(&itr_node->node_lock);
			#endif 

			free(INS_INDEX)	;
			return list; 
		}
		#ifndef _NO_HOH_LOCK_TRIE
		struct node* temp = itr_node;
		#endif
		itr_node = itr_node->children[index]; 
			
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_lock(&itr_node->node_lock); 
		Pthread_mutex_unlock(&temp->node_lock);
		#endif 
	} 
	
	// If prefix is present as a word. 
	bool isWord = (itr_node->is_end == true); 

	// If prefix is last node of tree (has no children) 
	bool isLast = isLastNode(itr_node); 

	// If prefix is present as a word, but 
	// there is no subtree below the last 
	// matching node. 
	if (isWord && isLast) 
	{   
		// Initialising the list[INS_INDEX] and increamenting the INS_INDEX pointer
		list[*INS_INDEX] = malloc((strlen(prefix)+2)*sizeof(char)); 
		list[*INS_INDEX] = prefix; 
		(*INS_INDEX)++;
		list[*INS_INDEX]  = NULL; 

		#ifdef _NO_HOH_LOCK_TRIE
		#ifndef _S_LOCK_TRIE 
		pthread_rwlock_unlock(&wr_lock); 
		#endif 
		#endif

		#ifdef _NO_HOH_LOCK_TRIE
		#ifdef _S_LOCK_TRIE 
		Pthread_mutex_unlock(&global_lock);
		#endif 
		#endif

		#ifndef _NO_HOH_LOCK_TRIE	
		Pthread_mutex_unlock(&itr_node->node_lock);
		#endif 

		free(INS_INDEX)	;
		return list; 
	} 

	// If there are are nodes below last matching character. 
	if (!isLast) 
	{ 	
		// Recursive helper function to find the all the words with matching prefix
		suggestionsRec(itr_node, prefix ,list,INS_INDEX); 
		list[*INS_INDEX] = NULL; 

		#ifdef _NO_HOH_LOCK_TRIE
		#ifndef _S_LOCK_TRIE 
		pthread_rwlock_unlock(&wr_lock); 
		#endif 
		#endif 


		#ifdef _NO_HOH_LOCK_TRIE
		#ifdef _S_LOCK_TRIE 
		Pthread_mutex_unlock(&global_lock);
		#endif 
		#endif
		#ifndef _NO_HOH_LOCK_TRIE	
		Pthread_mutex_unlock(&itr_node->node_lock);
		#endif 

		free(INS_INDEX)	;
        return list ;
	}
   
	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
	Pthread_mutex_unlock(&global_lock);
	#endif 
	#endif

	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_unlock(&wr_lock); 
	#endif 
	#endif

	#ifndef _NO_HOH_LOCK_TRIE	
	Pthread_mutex_unlock(&itr_node->node_lock);
	#endif 

	free(INS_INDEX);
    return list ;
}

/*
    delete_trie_helper(struct node* itr_node) helper function to delete_trie,
	Recursively clears all the nodes and their locks
*/

void delete_trie_helper(struct node* itr_node){
	if(itr_node == NULL ) return ;
	
	for(int level = 0 ; level<ALPHABET_SIZE; level++){
		if(itr_node->children[level] !=NULL){
			delete_trie_helper(itr_node->children[level]); 
		}
	}

	if(isLastNode(itr_node) || itr_node != NULL){
		#ifndef _NO_HOH_LOCK_TRIE	
		pthread_mutex_destroy(&itr_node->node_lock);
		#endif 
		free(itr_node); 
		itr_node = NULL; 
		return ; 
	}	
}

/*
    delete_trie(trie_t trie) clears the entire trie from memory. 
*/

void delete_trie(trie_t trie){
	
	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
	Pthread_mutex_lock(&global_lock);
	#endif 
	#endif

	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_wrlock(&wr_lock); 
	#endif 
	#endif 
	
	if(trie == NULL || trie->head == NULL || isLastNode(trie->head)){
		#ifdef _NO_HOH_LOCK_TRIE
		#ifdef _S_LOCK_TRIE 
		Pthread_mutex_unlock(&global_lock);
		#endif 
		#endif 

		#ifdef _NO_HOH_LOCK_TRIE
		#ifndef _S_LOCK_TRIE 
		pthread_rwlock_unlock(&wr_lock); 
		#endif 
		#endif 
		
		#ifndef _NO_HOH_LOCK_TRIE	
		pthread_mutex_destroy(&trie->head->node_lock);
		#endif 
		
		free(trie->head);
		free(trie);
		trie->head = NULL;
		trie = NULL;
		return ; 
	}
	struct node* itr_node = trie->head; 
	
	delete_trie_helper(itr_node);
	

	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
		Pthread_mutex_unlock(&global_lock);
	#endif 
	#endif

	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_unlock(&wr_lock); 
	#endif 
	#endif 

	free(trie);
	trie = NULL; 
	return ; 
}

