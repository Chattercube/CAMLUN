#ifndef GRAPHEDGEKEY_TYPE_H
#define GRAPHEDGEKEY_TYPE_H

#include "typemethods.h"
#include "graph.h"

void *graphedgekey_default_constructor();
void graphedgekey_destructor(void *ptr);
void *graphedgekey_copy_constructor(void *ptr);
int graphedgekey_comparator(void *first, void *second);
size_t graphedgekey_hash_function(void *ptr);

#endif