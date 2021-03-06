/* 
	counters.c

	Jason Chen
	CS50, February 1, 2021

	This is the implementation of the module for counters. See counters.h for more information.
	Referenced bag.c

*/

#include <stdio.h>
#include <stdlib.h>
#include "counters.h"
#include "memory.h"

typedef struct countersnode
{
	int key;
	int counter;
	struct countersnode *next;
} countersnode_t;


typedef struct counters {
	struct countersnode *head;
} counters_t;

// initialize a countersnode
static countersnode_t * 
countersnode_new(int key) {
	countersnode_t *node = count_malloc(sizeof(countersnode_t));

	if (node == NULL) {
		// error allocating memory for node
		return NULL;
	} else {
		// create a new countersnode with designated key value and counter = 1
		node->key = key;
		node->counter = 1;
		node->next = NULL;
		return node;
	}
}

// create a new set of counters
counters_t *
counters_new(void)
{
	counters_t *counters = count_malloc(sizeof(counters_t));
	if (counters == NULL) {
		return NULL;
	} else {
		// initialize contents of counters structure
		counters->head = NULL;
		return counters;
	}

}

// returns the node with desired key
static countersnode_t *find_node(counters_t *ctrs, const int key) {

	countersnode_t *node = ctrs->head;

	// loop through nodes to check if key exists and return that node
	while (node != NULL) {
		if (node->key == key) {
			return node;
		}
		node = node->next;
	}

	return NULL;  // key does not exist, so return NULL

}

// increment the counter indicated by key or add a new counter if the key does not exist
int counters_add(counters_t *ctrs, const int key) {

	if (ctrs != NULL && key >= 0) {

		countersnode_t *node = find_node(ctrs, key);

		// if key does not exist, create new node with that key
		if (node == NULL) {
			countersnode_t *new = countersnode_new(key);
			
			if (new != NULL) {
				// add to head of the list
				new->next = ctrs->head;
				ctrs->head = new;
				return 1;
			}
		// increment the counter by 1
		} else {
			node->counter++;
			return node->counter;
		}

	} else {
		return 0;
	}
}

// Return current value of counter associated with the given key
int counters_get(counters_t *ctrs, const int key) {
	if (ctrs != NULL && key >= 0) {
		countersnode_t *node = find_node(ctrs, key);
		
		// if the key is found, return the counter value 
		if (node != NULL) {
			return node->counter;
		}
	}
	return 0;
}


// set the current value of counter associated with the given key
bool counters_set(counters_t *ctrs, const int key, const int count) {
	if (ctrs != NULL && key >= 0 && count >= 0) {

		countersnode_t *node = find_node(ctrs, key);

		// if the key does not exist, create new node
		if (node == NULL) {
			countersnode_t *new = countersnode_new(key);
			if (new != NULL) {
				// add to head of the list
				new->next = ctrs->head;
				ctrs->head = new;
				new->counter = count;
			}

		// else update the counter of the identified node
		} else {
			node->counter = count;
		}
		return true;
	}

	return false;
}


// Print all counters, provide the output file
void counters_print(counters_t *ctrs, FILE *fp) {
	if (fp != NULL) {
		if (ctrs != NULL) {
			fputc('{', fp);
			for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
				// print this node
				fprintf(fp, "%d = %d", node->key, node->counter);
				if (node->next != NULL) {
					fputc(',', fp);
				}
			}
			fputc('}', fp);
			fputc('\n', fp);
		} else {
			fprintf(fp, "(null)");
		}
	}
}

// iterate each item in counter one at a time
void
counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count) )
{
  if (ctrs != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->counter);

    }
  }
}

// Delete the whole counterset
void counters_delete(counters_t *ctrs) {
	if (ctrs != NULL) {
		for (countersnode_t *node = ctrs->head; node != NULL;) {
			countersnode_t *next = node->next;
			count_free(node);
			node = next;
		}
		count_free(ctrs);
	}
}























