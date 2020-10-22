// http://ianfinlayson.net/class/cpsc425/notes/08-read-write
// https://chameerawijebandara.wordpress.com/2014/07/12/linked-list-with-read-write-locks-for-the-entire-linked-list-in-c/

#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include <string.h>
#include <stdbool.h> 
#include "common_threads.h"
int INS_INDEX = 0 ; 

trie_t init_trie(void){
    // Write your code here
    _trie_t* trie =  ( _trie_t*) malloc(sizeof( _trie_t)); 
	if(trie == NULL){
		printf("unable to create trie\n"); 
		return NULL; 
	}
    trie_node_t head = (trie_node_t)malloc(sizeof(_trie_node_t)); 

	if(head == NULL){
		printf("unable to create trie head\n"); 
		return NULL; 
	}

    trie->head = head ; 
	trie->head->is_end = false; 
	for (int i = 0; i < ALPHABET_SIZE; i++) 
		trie->head->children[i] = NULL; 

	
	#ifndef _NO_HOH_LOCK_TRIE

	Pthread_mutex_init(&trie->head->node_lock,NULL); 

	#endif
	return trie; 
} 

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


// Returns 0 if current node has a child 
// If all children are NULL, return 1. 
bool isLastNode(struct node* trie_node){ 

	for (int i = 0; i < ALPHABET_SIZE; i++){
		if (trie_node->children[i]){ 
			return 0; 
		}
	}
		
	return 1; 
} 

void insert(trie_t trie, char* key, int value){
    // Write your code here
	#ifndef _S_LOCK_TRIE
	Pthread_mutex_lock(&trie->head->node_lock);
	#endif 
	
    struct node* ins_node = trie->head; 

	for (int level = 0; level < strlen(key); level++) 
	{ 
		int index = CHAR_TO_INDEX(key[level]); 
		if (ins_node->children[index] == NULL){ 
			ins_node->children[index] = create_node(); 
		}
		ins_node = ins_node->children[index]; 
	} 
    ins_node->value = value ; 
	// mark last node as leaf 
	ins_node->is_end = true; 
	#ifndef _S_LOCK_TRIE
	Pthread_mutex_unlock(&trie->head->node_lock);
	#endif 
}

int find(trie_t trie,char* key, int* val_ptr){
    // Write your code here
    int length = strlen(key); 
    
	#ifndef _S_LOCK_TRIE
	Pthread_mutex_lock(&trie->head->node_lock);
	#endif 

	struct node* itr_node = trie->head ; 
	for (int level = 0; level < length; level++) 
	{ 

		int index = CHAR_TO_INDEX(key[level]); 
		if (!itr_node->children[index]){
			#ifndef _S_LOCK_TRIE
			Pthread_mutex_unlock(&trie->head->node_lock);
			#endif 
			return -1; 
		} 
		itr_node = itr_node->children[index]; 
	} 
    if(itr_node != NULL && itr_node->is_end){
        *val_ptr = itr_node->value ; 
		#ifndef _S_LOCK_TRIE
		Pthread_mutex_unlock(&trie->head->node_lock);
		#endif 
        return 0 ; 
    }

	#ifndef _S_LOCK_TRIE
	Pthread_mutex_unlock(&trie->head->node_lock);
	#endif 

    return -1;
} 

// int delete_kv_helper(struct node* itr_node, char* key){
// 	if (itr_node == NULL)
// 		return 0;

// 	// if we have not reached the end of the string
// 	if (*key)
// 	{
// 		// recur for the node corresponding to next character in
// 		// the string and if it returns 1, delete current node
// 		// (if it is non-leaf)
// 		if (itr_node != NULL && (itr_node)->children[*key - 'a'] != NULL &&
// 			delete_kv_helper(((itr_node)->children[*key - 'a']), key + 1) &&
// 			(itr_node)->is_end == 0)
// 		{
// 			if (isLastNode(itr_node))
// 			{
// 				free(itr_node);
// 				(itr_node) = NULL;
// 				return 1;
// 			}
// 			else {
// 				return 0;
// 			}
// 		}
// 	}

// 	// if we have reached the end of the string
// 	if (*key == '\0' )
// 	{
// 		// if current node is a leaf node and don't have any children
// 		if (isLastNode(itr_node))
// 		{
// 			free(itr_node); // delete current node
// 			(itr_node) = NULL;
// 			return 1; // delete non-leaf parent nodes
// 		}

// 		// if current node is a leaf node and have children
// 		else
// 		{
// 			// mark current node as non-leaf node (DON'T DELETE IT)
// 			(itr_node)->is_end = 0;
// 			return 0;	   // don't delete its parent nodes
// 		}
// 	}

// 	return 0;

// }

// void delete_kv(trie_t trie, char* key){
//     // Write your code here
// 	struct node* itr_node = trie->head; 
// 	delete_kv_helper (itr_node, key); 
// }


struct node* delete_kv_helper(struct node* itr_node, char *key){
	
	if(itr_node == NULL)
		return itr_node;
	
	if(*key){
		itr_node->children[(CHAR_TO_INDEX(*key))] = delete_kv_helper(itr_node->children[CHAR_TO_INDEX(*key)], key+1);
		
		if(itr_node->is_end == 0 && isLastNode(itr_node) ){
			free(itr_node);
			itr_node = NULL;
		}
	}

	if(*key == '\0'){

		itr_node->is_end = 0;

		if(isLastNode(itr_node)){
			free(itr_node);
			itr_node = NULL;
		}
		return itr_node;
	}
	return itr_node;
}

void delete_kv(trie_t trie, char* key){
		#ifndef _S_LOCK_TRIE
		Pthread_mutex_lock(&trie->head->node_lock);
		#endif 
    	struct node* itr_node = trie->head; 
		delete_kv_helper (itr_node, key);
		#ifndef _S_LOCK_TRIE
		Pthread_mutex_unlock(&trie->head->node_lock);
		#endif 

		return ; 
}



// Recursive function to print auto-suggestions for given 
// node. 

char *append (char *slice, char part) {

	char *str = malloc (sizeof (char) * (strlen (slice) + 2));
	
	int i = 0;
	while (slice[i] != '\0') str[i] = slice[i], i++;

	str[i++] = part;
	str[i] = '\0';

	return str;
}


void suggestionsRec(struct node* trie_node, char* currPrefix, char** list) 
{ 
	// found a string in Trie with the given prefix 
	if (trie_node->is_end) 
	{ 
		
		list[INS_INDEX] = malloc(strlen(currPrefix)*sizeof(char)); 
		list[INS_INDEX] = currPrefix; 
		INS_INDEX++; 
        // printf("%s\n", list[INS_INDEX]); 
        // printf("%d\n", INS_INDEX);  
	} 

	// All children struct node pointers are NULL 
	if (isLastNode(trie_node)) 
		return; 

	for (int i = 0; i < ALPHABET_SIZE; i++) 
	{ 
		if (trie_node->children[i]) 
		{ 
			if (trie_node->children[i]!= NULL) 
			{ 
			suggestionsRec(trie_node->children[i], append(currPrefix, i+97), list); 
			} 
		}	 
	} 
}

char** keys_with_prefix(trie_t trie, char* prefix){
    // Write your code here

	#ifndef _S_LOCK_TRIE
	Pthread_mutex_lock(&trie->head->node_lock);
	#endif 

    INS_INDEX = 0; 
    char** list = malloc(2048*sizeof(char*));  
	struct node* itr_node = trie->head; 
	if(itr_node == NULL){
		#ifndef _S_LOCK_TRIE
		Pthread_mutex_unlock(&trie->head->node_lock);
		#endif 
		return list; 
	}
	// Check if prefix is present and find the 
	// the node (of last level) with last character 
	// of given string. 
	int level; 
	int n = strlen(prefix); 
	for (level = 0; level < n; level++) 
	{ 
		int index = CHAR_TO_INDEX(prefix[level]); 
		
		// no string in the Trie has this prefix 
		if (!itr_node->children[index]) {
			#ifndef _S_LOCK_TRIE
			Pthread_mutex_unlock(&trie->head->node_lock);
			#endif 
			return list; 
		}
			
		
		itr_node = itr_node->children[index]; 
	} 

    int * index = (int*) malloc(sizeof(int)); 
    *index = 0; 
	// If prefix is present as a word. 
	bool isWord = (itr_node->is_end == true); 

	// If prefix is last node of tree (has no 
	// children) 
	bool isLast = isLastNode(itr_node); 

	// If prefix is present as a word, but 
	// there is no subtree below the last 
	// matching node. 
	if (isWord && isLast) 
	{   
		// printf("%s\n", prefix); 
		list[INS_INDEX] = malloc(strlen(prefix)*sizeof(char)); 
		list[INS_INDEX] = prefix; 
		INS_INDEX++;
		list[INS_INDEX]  = NULL; 
		#ifndef _S_LOCK_TRIE
		Pthread_mutex_unlock(&trie->head->node_lock);
		#endif 
		return list; 
	} 
	// If there are are nodes below last 
	// matching character. 
	if (!isLast) 
	{ 
		suggestionsRec(itr_node, prefix ,list); 
		list[INS_INDEX] = NULL; 
		#ifndef _S_LOCK_TRIE
		Pthread_mutex_unlock(&trie->head->node_lock);
		#endif 
        return list ;
	}
   
	#ifndef _S_LOCK_TRIE
	Pthread_mutex_unlock(&trie->head->node_lock);
	#endif 
    return list ;
}


void delete_trie_helper(struct node* itr_node){
	if(itr_node == NULL ) return ;
	
	for(int level = 0 ; level<ALPHABET_SIZE; level++){
		if(itr_node->children[level] !=NULL){
			delete_trie_helper(itr_node->children[level]); 
		}
	}
	if(isLastNode(itr_node)){
		free(itr_node); 
		itr_node = NULL; 
		return ; 
	}	
}

void delete_trie(trie_t trie){
    // Write your code here
	#ifndef _S_LOCK_TRIE
	Pthread_mutex_lock(&trie->head->node_lock);
	#endif 
	struct node* itr_node = trie->head; 
	if(isLastNode(itr_node)){
		free(trie);
		trie = NULL;
		#ifndef _S_LOCK_TRIE
		Pthread_mutex_unlock(&trie->head->node_lock);
		#endif 
		return ; 
	}
	delete_trie_helper(itr_node);
	free(trie);
	trie = NULL; 
	#ifndef _S_LOCK_TRIE
	Pthread_mutex_unlock(&trie->head->node_lock);
	#endif 
	
	return ; 
}

void printList(char ** temp){ 
    for(int i= 0; i < INS_INDEX; i++) {
        printf("%s\n", temp[i]); 
    }
}


// int main(){
//     trie_t trie = init_trie(); 
//     insert(trie, "hello",0); 
// 	insert(trie, "dog",1); 
// 	insert(trie, "hell",0); 
// 	insert(trie, "cat",0); 
// 	insert(trie, "a",0); 
// 	insert(trie, "hel",0); 
// 	insert(trie, "help",0); 
// 	insert(trie, "helps",120); 
// 	insert(trie, "a",0); 
// 	insert(trie, "aaa",0); 
// 	insert(trie, "aa",0); 
// 	insert(trie, "aaaa",0); 
// 	insert(trie, "a",0); 
// 	insert(trie, "aaa",0); 
//     int* ptr = (int*)malloc(sizeof(int)) ;
// 	delete_kv(trie, "helps"); 
// 	delete_kv(trie, "help"); 
// 	printf("%d\n", find(trie, "helps", ptr)) ; 
// 	printf("%d\n", find(trie, "help", ptr)) ; 
// 	printf("%d\n", find(trie, "helping", ptr)) ; 
// 	printf("%d\n", find(trie, "hel", ptr)) ; 
// 	printf("%d\n", find(trie, "dog", ptr)) ; 
//     char ** temp = keys_with_prefix(trie, "a"); 
//     printList(temp);
//     delete_trie(trie); 
//     return 0 ; 
// }