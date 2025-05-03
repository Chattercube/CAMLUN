#ifndef TREESET_TYPE_H
#define TREESET_TYPE_H

#include "typemethods.h"
#include "treeset.h"

// ==== Methods Overview ==== 

// Access or iterate :

int treeset_compare(TreeSet *first, TreeSet *second);
size_t treeset_hash(TreeSet *this);

// Type methods :

void *treeset_default_constructor();
void treeset_destructor(void *ptr);
void *treeset_copy_constructor(void *ptr);
int treeset_comparator(void *first, void *second);
size_t treeset_hash_function(void *ptr);

// ==== End of Methods Overview ====

#endif