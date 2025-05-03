#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

// size_t hashmap_probe_next(HashMap *map, size_t index, size_t probe_count);
// bool hashmap_need_rehash(HashMap *map, size_t new_size);

// Constructors and destructors :

HashMap *hashmap_create(type_methods *key_methods, type_methods *value_methods);
HashMap *hashmap_destroy(HashMap *this);

// Access and iteration :

void *hashmap_get(HashMap *map, void *key);
bool hashmap_contains(HashMap *map, void *key);

HashMapNode *hashmap_find(HashMap *map, void *key);

// Capacity :

bool hashmap_empty(HashMap *map);
size_t hashmap_size(HashMap *map);
size_t hashmap_occupied_size(HashMap *map);
size_t hashmap_capacity(HashMap *map);

void hashmap_rehash(HashMap *map, size_t new_capacity);

// Modifiers :

void hashmap_add(HashMap *map, void *key);
void hashmap_set(HashMap *map, void *key, void *value);
void hashmap_reset(HashMap *map, void *key);
void hashmap_remove(HashMap *map, void *key);

// Copy constructors and creators :

HashMap *hashmap_clone(HashMap *this, type_methods *new_data_methods);

// ==== End of Method Overview ====

// ==== Macros ====

#define HASHMAP_KEYS_FOREACH(map, varname, callback)           \
    do {                                                       \
        for (size_t _i = 0; _i < map->capacity; _i++) {        \
            if (map->nodes[_i].status == HASHMAP_NODE_FILLED) { \
                varname = map->nodes[_i].key;                   \
                callback;                                      \
            }                                                  \
        }                                                      \
    } while (0)

#define HASHMAP_VALUES_FOREACH(map, varname, callback)         \
    do {                                                       \
        for (size_t _i = 0; _i < map->capacity; _i++) {        \
            if (map->nodes[_i].status == HASHMAP_NODE_FILLED) { \
                varname = map->nodes[_i].value;                 \
                callback;                                      \
            }                                                  \
        }                                                      \
    } while (0)

#define HASHMAP_PAIRS_FOREACH(map, keyname, valuename, callback) \
    do {                                                         \
        for (size_t _i = 0; _i < map->capacity; _i++) {          \
            if (map->nodes[_i].status == HASHMAP_NODE_FILLED) {   \
                keyname = map->nodes[_i].key;                     \
                valuename = map->nodes[_i].value;                 \
                callback;                                        \
            }                                                    \
        }                                                        \
    } while (0)

#define HASHMAP_PRINTF(map, keyname, valuename, ...)     \
    do {                                                 \
        printf("{");                                     \
        size_t _counter = 0;                             \
        HASHMAP_PAIRS_FOREACH(map, keyname, valuename, { \
            _counter++;                                  \
            printf(__VA_ARGS__);                         \
            if (_counter < hashmap_size(map)) {          \
                printf(", ");                            \
            }                                            \
        });                                              \
        printf("}");                                     \
    } while (0)

#define HASHMAP_FPRINTF(stream, map, keyname, valuename, ...) \
    do {                                                      \
        fprintf(stream, "{");                                 \
        size_t _counter = 0;                                  \
        HASHMAP_PAIRS_FOREACH(map, keyname, valuename, {      \
            _counter++;                                       \
            fprintf(stream, __VA_ARGS__);                     \
            if (_counter < hashmap_size(map)) {               \
                fprintf(stream, ", ");                        \
            }                                                 \
        });                                                   \
        fprintf(stream, "}");                                 \
    } while (0)

#define HASHMAP_SPRINTF(buffer, map, keyname, valuename, ...) \
    do {                                                      \
        sprintf(buffer, "{");                                 \
        size_t _counter = 0;                                  \
        HASHMAP_PAIRS_FOREACH(map, keyname, valuename, {      \
            _counter++;                                       \
            sprintf(buffer + strlen(buffer), __VA_ARGS__);    \
            if (_counter < hashmap_size(map)) {               \
                sprintf(buffer + strlen(buffer), ", ");       \
            }                                                 \
        });                                                   \
        sprintf(buffer + strlen(buffer), "}");                \
    } while (0)
// ==== End of Macros ====

#endif