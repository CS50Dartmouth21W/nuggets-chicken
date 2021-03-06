/*
set.c

Jason Chen
CS 50, February 1, 2021

This is the implementation of the module for set structure
Referenced bag.c

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "memory.h"

typedef struct setnode
{
	char *key;
	void *item;
	struct setnode *next;
} setnode_t;


typedef struct set {
	struct setnode *head;
} set_t;


// create a new empty set
set_t *set_new(void) {
	set_t *set = count_malloc(sizeof(set_t));

	if (set == NULL) {
		return NULL;

	} else {
		set->head = NULL;
		return set;
	}
}

// initialize a countersnode
static setnode_t * 
setnode_new(const char *key, void *item) {
	setnode_t *node = count_malloc(sizeof(setnode_t));

	if (node == NULL) {
		// error allocating memory for node
		return NULL;
	} else {
		// allocate memory for a copy of the string
		int length = strlen(key);
		char *k=(char*)malloc((length+1));
  		strcpy(k, key);

		// set the node's key, item, and next
		node->key = k;
		node->item = item;
		node->next = NULL;
		return node;
	}
}

// returns the node with desired key
static setnode_t *find_node(set_t *set, const char *key) {

	setnode_t *node = set->head;

	// loop through nodes to check if key exists
	while (node != NULL) {
		// if (node->key == key) {
		if (strcmp(node->key, key) == 0) {
			return node;
		}
		node = node->next;
	}

	return NULL;
}


// Insert item, identified by a key (string), into the given set
bool set_insert(set_t *set, const char *key, void *item) {
	if (set != NULL && key != NULL && item != NULL) {
		setnode_t *node = find_node(set, key);
		
		// if the node does not exist, create a new set node with desired key and item
		if (node == NULL) {
			setnode_t *new = setnode_new(key, item);
			if (new != NULL) {
				new->next = set->head;
				set->head = new;
				return true;
			}
		}
	}
	return false;
}

// Return the item associated with the given key
void *set_find(set_t *set, const char *key) {
	if (set != NULL && key != NULL) {
		setnode_t *node = find_node(set, key);

		if (node != NULL) {
			return node->item;
		}
	}
	return NULL;
}

// Print the whole set; provide the output file and func to print each item
void 
set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
	if (fp != NULL) {
	    if (set != NULL) {
	      	fputc('{', fp);
	      	for (setnode_t *node = set->head; node != NULL; node = node->next) {
	        // print this node
	        	if (itemprint != NULL) { // print the node's item 
	          		(*itemprint)(fp, node ->key, node->item); 
	          		if (node->next != NULL) {
					  fputc(',', fp);
					}
	        	}
	      	}
	      	fputc('}', fp);
	    } else {
	      fputs("(null)", fp);
	    }
		fputc('\n', fp);
  	}
}


// Iterate over the set, calling a function on each one
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) )
{
  if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->item); 
    }
  }
}



void set_delete(set_t *set, void (*itemdelete)(void *item)) {
  if (set != NULL) {
    for (setnode_t *node = set->head; node != NULL; ) {
      if (itemdelete != NULL) {         // if possible...
        (*itemdelete)(node->key);      // delete node's key
      }
      setnode_t *next = node->next;     // remember what comes next
      count_free(node);                 // free the node
      node = next;                      // and move on to next
    }

    count_free(set);
  }

#ifdef MEMTEST
  count_report(stdout, "End of set_delete");
#endif
}


