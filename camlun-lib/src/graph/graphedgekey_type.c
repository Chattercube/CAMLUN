#include "graphedgekey_type.h"

void *graphedgekey_default_constructor() {
    GraphEdgeKey *edge_key = malloc(sizeof(GraphEdgeKey));
    if (edge_key == NULL) {
        return NULL;
    }
    edge_key->from = NULL;
    edge_key->to = NULL;
    return edge_key;
}

void graphedgekey_destructor(void *ptr) {
    if (ptr) free(ptr);
}

void *graphedgekey_copy_constructor(void *ptr) {
    if (!ptr) return NULL;
    GraphEdgeKey *new_edge_key = malloc(sizeof(GraphEdgeKey));
    if (new_edge_key == NULL) {
        return NULL;
    }
    memcpy(new_edge_key, ptr, sizeof(GraphEdgeKey));
    return new_edge_key;
}

int graphedgekey_comparator(void *first, void *second) {
    if (!first && !second) return 0;
    if (!first) return -1;
    if (!second) return 1;
    GraphEdgeKey *edge_key1 = (GraphEdgeKey *)first;
    GraphEdgeKey *edge_key2 = (GraphEdgeKey *)second;
    int cmp_from = (edge_key1->from > edge_key2->from) - (edge_key1->from < edge_key2->from);
    int cmp_to = (edge_key1->to > edge_key2->to) - (edge_key1->to < edge_key2->to);
    return (cmp_from) ? cmp_from : cmp_to;
}

size_t graphedgekey_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(GraphEdgeKey), ptr);
}