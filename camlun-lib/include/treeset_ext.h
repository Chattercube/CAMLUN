#ifndef TREESET_EXT_H
#define TREESET_EXT_H

#include "treeset.h"

// ==== Methods Overview ====

// Access and iteration :

bool treeset_subset(TreeSet *this, TreeSet *another);
bool treeset_equals(TreeSet *this, TreeSet *another);

// Modifiers :

void treeset_add_set(TreeSet *this, TreeSet *another);
void treeset_remove_set(TreeSet *this, TreeSet *another);

// Copy constructors or creators : 

TreeSet *treeset_union(TreeSet *first, TreeSet *second);
TreeSet *treeset_intersection(TreeSet *first, TreeSet *second);
TreeSet *treeset_complement(TreeSet *first, TreeSet *second);

// ==== End of Methods Overview ====



#endif