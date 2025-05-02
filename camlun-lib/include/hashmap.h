#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdbool.h>
#include "typemethods.h"

/**
 * Constants
 */

#define HASHMAP_NODE_FREE 0x00
#define HASHMAP_NODE_FILLED 0xFF
#define HASHMAP_NODE_DELETED 0x01

static const size_t HASHMAP_INITIAL_CAPACITY = 64;
static const double HASHMAP_LOAD_FACTOR = 0.75;

/**
 * Type Structures
 */

typedef struct HashMapNode {
    void *key;
    void *value;
    unsigned char status;
} HashMapNode;

typedef struct HashMap {
    HashMapNode *nodes;
    size_t occupied_size;
    size_t size;
    size_t capacity;
    type_methods *key_methods;
    type_methods *value_methods;
} HashMap;


// ==== Method Overview ====

// Private Methods :

size_t hashmap_probe_next(HashMap *map, size_t index, size_t probe_count);
bool hashmap_need_rehash(HashMap *map, size_t new_size);

// Constructors and destructors :

HashMap *hashmap_create(type_methods *key_methods, type_methods *value_methods);
HashMap *hashmap_destroy(HashMap *this);

// Access and iteration :

HashMap *hashmap_get(HashMap *map, void *key);
bool hashmap_contains(HashMap *map, void *key);

// Capacity :

bool hashmap_empty(HashMap *map);
size_t hashmap_size(HashMap *map);
size_t hashmap_occupied_size(HashMap *map);
size_t hashmap_capacity(HashMap *map);

void hashmap_rehash(HashMap *map, size_t new_capacity);

// Modifiers :

void hashmap_set(HashMap *map, void *key, void *value);
void hashmap_reset(HashMap *map, void *key);
void hashmap_remove(HashMap *map, void *key);

// ==== End of Method Overview ====



// ==== Macros ====

#define HASHMAP_KEYS_FOREACH(map, varname, callback) \
    do {                                                            \
        for (size_t _i = 0; _i < map->capacity; _i++) {             \
            if (map->nodes[i].status == HASHMAP_NODE_FILLED) {      \
                varname = map->nodes[i].key;                        \
                callback;                                           \
            }                                                       \
        }                                                           \
    } while (0)

#define HASHMAP_VALUES_FOREACH(map, varname, callback) \
    do {                                                            \
        for (size_t _i = 0; _i < map->capacity; _i++) {             \
            if (map->nodes[i].status == HASHMAP_NODE_FILLED) {      \
                varname = map->nodes[i].value;                      \
                callback;                                           \
            }                                                       \
        }                                                           \
    } while (0)

#define HASHMAP_PAIRS_FOREACH(map, keyname, valuename, callback) \
    do {                                                            \
        for (size_t _i = 0; _i < map->capacity; _i++) {             \
            if (map->nodes[i].status == HASHMAP_NODE_FILLED) {      \
                keyname = map->nodes[i].key;                        \
                valuename = map->nodes[i].value;                    \
                callback;                                           \
            }                                                       \
        }                                                           \
    } while (0)

// ==== End of Macros ====



#endif