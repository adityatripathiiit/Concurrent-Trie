#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include <string.h>
#include <stdbool.h> 
#include "common_threads.h"


pthread_mutex_t global_lock;
pthread_rwlock_t wr_lock;

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


	
	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
	Pthread_mutex_init(&global_lock, NULL) ;
	#endif
	#endif

	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_init(&wr_lock, NULL); 
	#endif 
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
	
	#ifdef _NO_HOH_LOCK_TRIE
	#ifdef _S_LOCK_TRIE 
	Pthread_mutex_lock(&global_lock)  ;
	#endif 
	#endif


	#ifdef _NO_HOH_LOCK_TRIE
	#ifndef _S_LOCK_TRIE 
	pthread_rwlock_wrlock(&wr_lock); 
	#endif 
	#endif 

	struct node* ins_node = trie->head;

	#ifndef _NO_HOH_LOCK_TRIE
	Pthread_mutex_lock(&ins_node->node_lock); 
	
	#endif


	for (int level = 0; level < strlen(key); level++) 
	{ 
		int index = CHAR_TO_INDEX(key[level]); 
		if (ins_node->children[index] == NULL){ 
			ins_node->children[index] = create_node();
		}
		struct node* temp = ins_node;
		
		ins_node = ins_node->children[index]; 

		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_lock(&ins_node->node_lock); 
		Pthread_mutex_unlock(&temp->node_lock);
		#endif 
	
	} 

    ins_node->value = value ; 
	// mark last node as leaf 
	ins_node->is_end = true; 
	#ifndef _NO_HOH_LOCK_TRIE
	Pthread_mutex_unlock(&ins_node->node_lock); 
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
}

int find(trie_t trie,char* key, int* val_ptr){
    // Write your code here
    
	int length = strlen(key); 
    
	
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
		struct node* temp = itr_node;

		itr_node = itr_node->children[index]; 
		
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_lock(&itr_node->node_lock); 
		Pthread_mutex_unlock(&temp->node_lock);
		#endif 

	} 

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


struct node* delete_kv_helper(struct node* itr_node, char *key){
	
	if(itr_node == NULL){
		return itr_node;
	}
	
	#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_lock(&itr_node->node_lock);
	#endif

	if(*key){
		
		itr_node->children[(CHAR_TO_INDEX(*key))] = delete_kv_helper(itr_node->children[CHAR_TO_INDEX(*key)], key+1);

		if(itr_node->is_end == 0 && isLastNode(itr_node)){

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

void delete_kv(trie_t trie, char* key){
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

    	struct node* itr_node = trie->head; 
		
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_lock(&itr_node->node_lock ); 
		#endif

	
		for( int i=0; i <ALPHABET_SIZE; i++){
			if((*key == i+'a') && itr_node->children[i]){
				itr_node->children[i] =  delete_kv_helper (itr_node->children[i], key+1);
			}
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
		Pthread_mutex_unlock(&itr_node->node_lock ); 
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



void suggestionsRec(struct node* trie_node, char* currPrefix, char** list, int * INS_INDEX) 
{ 
	// found a string in Trie with the given prefix 
	if (trie_node->is_end) 
	{ 
		list[*INS_INDEX] = malloc((strlen(currPrefix)+2)*sizeof(char)); 
		strcpy(list[*INS_INDEX], currPrefix); 
		(*INS_INDEX)++; 
	} 

	// All children struct node pointers are NULL 
	if (isLastNode(trie_node)) 
		return; 

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

char** keys_with_prefix(trie_t trie, char* prefix){
    // Write your code here


    int* INS_INDEX = malloc(sizeof(int)); 
	*INS_INDEX = 0 ;
    char** list = malloc(2048*sizeof(char*));  
	if(list== NULL){
		printf("cant malloc "); 
		free(INS_INDEX);
		return list; 
	}
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
		struct node* temp = itr_node;
		itr_node = itr_node->children[index]; 
			
		#ifndef _NO_HOH_LOCK_TRIE
		Pthread_mutex_lock(&itr_node->node_lock); 
		Pthread_mutex_unlock(&temp->node_lock);
		#endif 
	} 
	
	// #ifndef _NO_HOH_LOCK_TRIE
	
	// Pthread_mutex_unlock(&itr_node->node_lock); 
	// #endif 
    
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
	// If there are are nodes below last 
	// matching character. 
	if (!isLast) 
	{ 	
		
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

void delete_trie(trie_t trie){
    // Write your code here
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

	// #ifndef _NO_HOH_LOCK_TRIE	
	// Pthread_mutex_lock(&trie->head->node_lock);
	// #endif 

	
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
		
		// #ifndef _NO_HOH_LOCK_TRIE
		// Pthread_mutex_unlock(&itr_node->node_lock);
		// #endif 

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

	// #ifndef _NO_HOH_LOCK_TRIE
	
	// // Pthread_mutex_unlock(&itr_node->node_lock);
	// #endif 
		

	free(trie);
	trie = NULL; 
	return ; 
}

// void printList(char ** temp){ 
//     for(int i= 0; i < 3; i++) {
//         printf("%s\n", temp[i]); 
//     }
// }


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
// 	// delete_kv(trie, "helps"); 
// 	// delete_kv(trie, "help"); 
// 	printf("%d\n", find(trie, "helps", ptr)) ; 
// 	printf("%d\n", find(trie, "help", ptr)) ; 
// 	printf("%d\n", find(trie, "helping", ptr)) ; 
// 	printf("%d\n", find(trie, "hel", ptr)) ; 
// 	printf("%d\n", find(trie, "dog", ptr)) ; 
//     char ** temp = keys_with_prefix(trie, "h"); 
//     printList(temp);
//     delete_trie(trie); 
//     return 0 ; 
// }