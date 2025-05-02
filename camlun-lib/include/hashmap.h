#include <stdlib.h>
#include <stdbool.h>
#include "typemethods.h"

#define HASHMAP_NODE_FREE 0x00
#define HASHMAP_NODE_FILLED 0xFF
#define HASHMAP_NODE_DELETED 0x01

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

static const size_t HASHMAP_INITIAL_CAPACITY = 64;

static const double HASHMAP_LOAD_FACTOR = 0.75;

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