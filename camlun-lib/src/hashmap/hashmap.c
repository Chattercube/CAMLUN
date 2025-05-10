#include "hashmap.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

// ==== Method Overview ====

// Private Methods :

size_t hashmap_probe_next(size_t capacity, size_t index, size_t probe_count);
bool hashmap_need_rehash(HashMap *map, size_t new_size);

// Constructors and destructors :

HashMap *hashmap_create(type_methods *key_methods, type_methods *value_methods);
void hashmap_destroy(HashMap *this);

// Access and iteration :

void *hashmap_get(HashMap *map, void *key);
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

// Macros

// === End of Method Overview ===

// Private methods

size_t hashmap_probe_next(size_t capacity, size_t index, size_t probe_count) {
    return (index + probe_count * probe_count) % capacity;
}

bool hashmap_need_rehash(HashMap *map, size_t new_size) {
    return (double)new_size / map->capacity > HASHMAP_LOAD_FACTOR;
}

// End of private methods

HashMap *hashmap_create(type_methods *key_methods, type_methods *value_methods) {
    HashMap *hashmap = malloc(sizeof(HashMap));
    if (hashmap == NULL) {
        return NULL;
    }
    hashmap->occupied_size = 0;
    hashmap->size = 0;
    hashmap->key_methods = key_methods;
    hashmap->value_methods = value_methods;

    hashmap->capacity = HASHMAP_INITIAL_CAPACITY;
    hashmap->nodes = calloc(hashmap->capacity, sizeof(HashMapNode));
    if (hashmap->nodes == NULL) {
        free(hashmap);
        return NULL;
    }
    return hashmap;
}

void hashmap_destroy(HashMap *map) {
    if (map == NULL) {
        return;
    }
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->nodes[i].status == HASHMAP_NODE_FILLED) {
            #if DEBUG
            printf("Destroying %s\n", map->nodes[i].key);
            #endif
            USE_DEL(map->key_methods, map->nodes[i].key);
            USE_DEL(map->value_methods, map->nodes[i].value);
        }
    }
    free(map->nodes);
    free(map);
    return;
}

void *hashmap_get(HashMap *map, void *key) {
    size_t index = USE_HASH(map->key_methods, key) % map->capacity;
    size_t probe_count = 0;
    while (map->nodes[index].status != HASHMAP_NODE_FREE) {
        if (map->nodes[index].status == HASHMAP_NODE_FILLED && USE_CMP(map->key_methods, map->nodes[index].key, key) == 0) {
            return map->nodes[index].value;
        }
        probe_count++;
        index = hashmap_probe_next(map->capacity, index, probe_count);
    }
    return NULL;
}

void *hashmap_get_key(HashMap *map, void *key) {
    size_t index = USE_HASH(map->key_methods, key) % map->capacity;
    size_t probe_count = 0;
    while (map->nodes[index].status != HASHMAP_NODE_FREE) {
        if (map->nodes[index].status == HASHMAP_NODE_FILLED && USE_CMP(map->key_methods, map->nodes[index].key, key) == 0) {
            return map->nodes[index].key;
        }
        probe_count++;
        index = hashmap_probe_next(map->capacity, index, probe_count);
    }
    return NULL;
}

bool hashmap_contains(HashMap *map, void *key) {
    size_t index = USE_HASH(map->key_methods, key) % map->capacity;
    size_t probe_count = 0;
    while (map->nodes[index].status != HASHMAP_NODE_FREE) {
        if (map->nodes[index].status == HASHMAP_NODE_FILLED && USE_CMP(map->key_methods, map->nodes[index].key, key) == 0) {
            return true;
        }
        probe_count++;
        index = hashmap_probe_next(map->capacity, index, probe_count);
    }
    return false;
}

// HashMapNode *hashmap_find(HashMap *map, void *key) {
//     size_t index = USE_HASH(map->key_methods, key) % map->capacity;
//     size_t probe_count = 0;
//     while (map->nodes[index].status != HASHMAP_NODE_FREE) {
//         if (map->nodes[index].status == HASHMAP_NODE_FILLED && USE_CMP(map->key_methods, map->nodes[index].key, key) == 0) {
//             return map->nodes + index;
//         }
//         probe_count++;
//         index = hashmap_probe_next(map, index, probe_count);
//     }
//     return NULL;
// }

void hashmap_rehash(HashMap *map, size_t new_capacity) {
    // if (map->occupied_size == 0) {
    //     map->capacity = new_capacity;
    //     return;
    // }

    HashMapNode *new_nodes = calloc(new_capacity, sizeof(HashMapNode));
    if (new_nodes == NULL) {
        return;
    }

    size_t new_size = 0;
    
    #if DEBUG
    // printf("Old size %lu\n",  map->size);
    #endif
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->nodes[i].status != HASHMAP_NODE_FILLED) {
            continue;
        }
        size_t index = USE_HASH(map->key_methods, map->nodes[i].key) % new_capacity;
        size_t probe_count = 0;
        while (new_nodes[index].status == HASHMAP_NODE_FILLED) {
            probe_count++;
            index = hashmap_probe_next(new_capacity, index, probe_count);
        }
        new_nodes[index].key = map->nodes[i].key;
        new_nodes[index].value = map->nodes[i].value;
        new_nodes[index].status = HASHMAP_NODE_FILLED;
        new_size++;
    }

    free(map->nodes);
    map->capacity = new_capacity;
    map->nodes = new_nodes;
    map->occupied_size = new_size;
    map->size = new_size;
    #if DEBUG
    // printf("New size %lu\n",  map->size);
    // HASHMAP_PRINTF(map, char *key, void *value, "%s", key); printf("\n");
    #endif
    
    return;
}

size_t hashmap_size(HashMap *map) {
    return map->size;
}

bool hashmap_empty(HashMap *map) {
    return map->size == 0;
}

size_t hashmap_occupied_size(HashMap *map) {
    return map->occupied_size;
}

size_t hashmap_capacity(HashMap *map) {
    return map->capacity;
}

void hashmap_add(HashMap *map, void *key) {
    if (hashmap_need_rehash(map, map->occupied_size + 1)) {
        hashmap_rehash(map, map->capacity * 2);
    }

    size_t index = USE_HASH(map->key_methods, key) % map->capacity;
    size_t probe_count = 0;
    while (map->nodes[index].status != HASHMAP_NODE_FREE) {
        if (map->nodes[index].status == HASHMAP_NODE_FILLED && USE_CMP(map->key_methods, map->nodes[index].key, key) == 0) {
            return;  // Key already exists
        }
        probe_count++;
        index = hashmap_probe_next(map->capacity, index, probe_count);
    }

    if (map->nodes[index].status == HASHMAP_NODE_FREE) {
        map->occupied_size++;
    }
    map->nodes[index].key = USE_DUP(map->key_methods, key);
    map->nodes[index].value = USE_CRT(map->value_methods);
    map->nodes[index].status = HASHMAP_NODE_FILLED;
    map->size++;
}

void hashmap_set(HashMap *map, void *key, void *value) {
    if (hashmap_need_rehash(map, map->occupied_size + 1)) {
        hashmap_rehash(map, map->capacity * 2);
    }

    size_t index = USE_HASH(map->key_methods, key) % map->capacity;
    size_t probe_count = 0;
    while (map->nodes[index].status != HASHMAP_NODE_FREE) {
        if (map->nodes[index].status == HASHMAP_NODE_FILLED && USE_CMP(map->key_methods, map->nodes[index].key, key) == 0) {
            USE_DEL(map->value_methods, map->nodes[index].value);
            map->nodes[index].value = USE_DUP(map->value_methods, value);
            return;
        }
        probe_count++;
        index = hashmap_probe_next(map->capacity, index, probe_count);
    }

    if (map->nodes[index].status == HASHMAP_NODE_FREE) {
        map->occupied_size++;
    }

    void *duplicated_key = USE_DUP(map->key_methods, key);
    fprintf(stderr, "Duplicating key %p %p\n", key, duplicated_key);
    map->nodes[index].key = duplicated_key;
    // map->nodes[index].key = map->key_methods->dup(key);
    map->nodes[index].value = USE_DUP(map->value_methods, value);
    map->nodes[index].status = HASHMAP_NODE_FILLED;
    map->size++;
    return;
}

void hashmap_reset(HashMap *map, void *key) {
    size_t index = USE_HASH(map->key_methods, key) % map->capacity;
    size_t probe_count = 0;
    while (map->nodes[index].status != HASHMAP_NODE_FREE) {
        if (map->nodes[index].status == HASHMAP_NODE_FILLED && USE_CMP(map->key_methods, map->nodes[index].key, key) == 0) {
            USE_DEL(map->value_methods, map->nodes[index].value);
            map->nodes[index].value = USE_CRT(map->value_methods);
            return;
        }
        probe_count++;
        index = hashmap_probe_next(map->capacity, index, probe_count);
    }
}

void hashmap_remove(HashMap *map, void *key) {
    size_t index = USE_HASH(map->key_methods, key) % map->capacity;
    size_t probe_count = 0;
    while (map->nodes[index].status != HASHMAP_NODE_FREE) {
        if (map->nodes[index].status == HASHMAP_NODE_FILLED && USE_CMP(map->key_methods, map->nodes[index].key, key) == 0) {
            USE_DEL(map->key_methods, map->nodes[index].key);
            USE_DEL(map->value_methods, map->nodes[index].value);
            map->nodes[index].status = HASHMAP_NODE_DELETED;
            map->size--;
            return;
        }
        probe_count++;
        index = hashmap_probe_next(map->capacity, index, probe_count);
    }
}

void hashmap_clear(HashMap *map) {
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->nodes[i].status == HASHMAP_NODE_FILLED) {
            USE_DEL(map->key_methods, map->nodes[i].key);
            USE_DEL(map->value_methods, map->nodes[i].value);
            map->nodes[i].status = HASHMAP_NODE_FREE;
        }
    }
    map->size = 0;
    map->occupied_size = 0;
}