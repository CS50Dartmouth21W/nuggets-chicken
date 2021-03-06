/*
hashtable.c

Jason Chen
CS50
January 27, 2021

This is the implementation of the hashtable module
referenced bag.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "set.h"
#include "jhash.h"
#include "memory.h"


typedef struct hashtable {
    set_t **array;  // pointer to pointer to the data
    int num_slots;  // number of slots in array
} hashtable_t;

// Create a new (empty) hashtable
hashtable_t *hashtable_new(const int num_slots) {
    if (num_slots > 0) {
        hashtable_t *ht = count_malloc(sizeof(hashtable_t));
        if (ht != NULL) {
            ht->num_slots = num_slots;
            ht->array = calloc(num_slots, sizeof(struct set_t*));
            return ht;
        }
    }
    return NULL;
}

// Insert item, identified by key (string), into the given hashtable
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht != NULL && key != NULL && item != NULL) {
        unsigned long index = JenkinsHash(key, ht->num_slots);
        
        // if the slot at index "index" of array is empty, point to a new set with a new item
        if (ht->array[index] == NULL) {
            set_t *set = set_new();
            if (set != NULL) {
                ht->array[index] = set;
                return set_insert(set, key, item);
            }
        } else {
            // insert the item in the set
            return set_insert(ht->array[index], key, item);
        }
    }
    return false;
}

// Return the item associated with the given key
void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht != NULL && key != NULL ) {
        unsigned long index = JenkinsHash(key, ht->num_slots);
        return set_find(ht->array[index], key);

    }
    return NULL;
}

// how to pass in itemrint for set_print
void hashtable_print(hashtable_t *ht, FILE *fp, 
                     void (*itemprint)(FILE *fp, const char *key, void *item))
{
    if (fp != NULL) {
        if (ht != NULL) {
            // print one line per slot if itemprint == NULL
            if (itemprint == NULL) {
                for (int i = 0; i < ht->num_slots; i++) {
                    fputs("{}\n", fp);
                }
            } else{
                // print out the set at each slot in array
                for (int i = 0; i < ht->num_slots; i++) {
                    set_print(ht->array[i], fp, itemprint);
                }
            }
        } else {
            fputs("(null)\n", fp);
        }
    }

}

// Iterate over all items in the table; in undefined order.
void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item) )                      
{
    if (ht != NULL && itemfunc != NULL) {
        for (int i = 0; i < ht->num_slots; i++) {
            if (ht->array[i] != NULL) {
                set_iterate(ht->array[i], arg, itemfunc);
            }
        }
    }
}

//---------not finished

void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
  if (ht != NULL) {
    for (int i = 0; i < ht->num_slots; i++ ) {
        if (ht->array[i] != NULL) {
            set_delete(ht->array[i], itemdelete);
        }
    }

    count_free(ht->array);
    count_free(ht);
  }

#ifdef MEMTEST
  count_report(stdout, "End of set_delete");
#endif
}